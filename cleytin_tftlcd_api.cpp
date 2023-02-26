#include "cleytin_tftlcd_api.h"

/**
 * @brief Função a ser executada antes de cada transação para configurar corretamente o pino de Comando/Dados
 * 
 * @param t A transação iniciando
 */
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int) t->user;
    gpio_set_level((gpio_num_t) LCD_PIN_NUM_DC, dc);
}

CleytinTFTAPI::CleytinTFTAPI() {
    esp_err_t ret;
    this->spi = new spi_device_handle_t();

    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(spi_bus_config_t));
    buscfg.miso_io_num = LCD_PIN_NUM_MISO;
    buscfg.mosi_io_num = LCD_PIN_NUM_MOSI;
    buscfg.sclk_io_num = LCD_PIN_NUM_CLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = LCD_WIDTH_PIXELS*LCD_HEIGHT_PIXELS*2+8;

    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(spi_device_interface_config_t));
    devcfg.clock_speed_hz = 26*1000*1000;           //Clock em 26 MHz
    devcfg.mode = 0;                                //SPI mode 0
    devcfg.spics_io_num = LCD_PIN_NUM_CS;
    devcfg.queue_size = 7;                          //Limite de transações em fila
    devcfg.pre_cb = lcd_spi_pre_transfer_callback;

    ret = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(HSPI_HOST, &devcfg, this->spi);
    ESP_ERROR_CHECK(ret);

    this->sendingBuffer = false;

    this->lcdInit();
}

CleytinTFTAPI::~CleytinTFTAPI() {
    delete this->spi;
}

uint32_t CleytinTFTAPI::getLcdId()
{
    this->sendCmd(0x04);

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8 * 3;
    t.flags = SPI_TRANS_USE_RXDATA;
    t.user = (void*) 1;

    esp_err_t ret = spi_device_polling_transmit(*this->spi, &t);
    assert( ret == ESP_OK );

    return *(uint32_t*) t.rx_data;
}

void CleytinTFTAPI::sendCmd(const uint8_t cmd) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    t.user = (void*) 0;
    assert(spi_device_polling_transmit(*this->spi, &t) == ESP_OK);
}

void CleytinTFTAPI::sendData(const uint8_t *data, int len) {
    spi_transaction_t t;
    if(len == 0) return;
    memset(&t, 0, sizeof(t));
    t.length = len * 8;
    t.tx_buffer = data;
    t.user = (void*) 1;
    assert(spi_device_polling_transmit(*this->spi, &t) == ESP_OK);
}

void CleytinTFTAPI::lcdInit() {
    const lcd_init_cmd_t* lcd_init_cmds;

    gpio_set_direction((gpio_num_t) LCD_PIN_NUM_DC, GPIO_MODE_OUTPUT);

    //Detetando tipo de LCD
    uint32_t lcd_id = this->getLcdId();
    printf("LCD ID: %d\n", lcd_id);
    if (lcd_id == 0) {
        //zero, ili
        lcd_init_cmds = ili_init_cmds;
    } else {
        // não zero, ST
        lcd_init_cmds = st_init_cmds;
    }

    int i = 0;
    while (lcd_init_cmds[i].cmd != 0) {
        this->sendCmd(lcd_init_cmds[i].cmd);
        this->sendData(lcd_init_cmds[i].data, lcd_init_cmds[i].sizeData);
        if (lcd_init_cmds[i].sizeData & 0x80) { // Se o comando exige pausa antes de continuar
            vTaskDelay(100 / portTICK_RATE_MS);
        }
        i++;
    }
}

bool CleytinTFTAPI::sendBuffer(uint16_t *buff) {
    if(this->sendingBuffer) {
        printf("CleytinTFTAPI tentativa de enviar quando ocupado");
        return false;
    }
    // Declarando estático pois precisamos dessa memória durante a transmissão que ainda vai estar
    // acontecendo após o termino da execução desse método
    static spi_transaction_t trans[6];
    memset(trans, 0, sizeof(spi_transaction_t) * 6);

    for (int x = 0; x < 6; x++) {
        trans[x].user = ((x & 1) == 0) ? (void *) 0 : (void *) 1;
        trans[x].length = ((x & 1) == 0) ? 8 : 8 * 4;
        trans[x].flags = SPI_TRANS_USE_TXDATA;
    }

    // Comando: Configurar culuna
    trans[0].tx_data[0] = 0x2A;
    // Parametros do comando
    trans[1].tx_data[0] = 0;                            //High byte da coordenada inicial
    trans[1].tx_data[1] = 0;                            //Low byte da coordenada inicial
    trans[1].tx_data[2] = LCD_WIDTH_PIXELS >> 8;        //High byte da coordenada final
    trans[1].tx_data[3] = LCD_WIDTH_PIXELS & 0xff;      //Low byte da coordenada final

    // Comando: Configurar linha
    trans[2].tx_data[0] = 0x2B;
    // Parametros do comando
    trans[3].tx_data[0] = 0;                            //High byte da coordenada inicial
    trans[3].tx_data[1] = 0;                            //Low byte da coordenada inicial
    trans[3].tx_data[2] = LCD_HEIGHT_PIXELS >> 8;       //High byte da coordenada final
    trans[3].tx_data[3] = LCD_HEIGHT_PIXELS & 0xff;     //Low byte da coordenada final

    // Comando: Escrever na memoria
    trans[4].tx_data[0] = 0x2C;
    // Parametro do comando, o buffer de pixels
    trans[5].tx_buffer = buff;
    trans[5].length = LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS * 16;
    trans[5].flags = 0;

    //Enfileirando todas as transações
    for (int x = 0; x < 6; x++) {
        if(spi_device_queue_trans(*this->spi, &trans[x], portMAX_DELAY) != ESP_OK) {
            return false;
        }
    }
    this->sendingBuffer = true;

    // Nesse momento o processo de enviar os pixels para a tela começou
    // A operação será executada em background via DMA sem muito impacto no processador
    return true;
}

void CleytinTFTAPI::waitBufferTransfer() {
    if(!this->sendingBuffer) {
        return;
    }
    spi_transaction_t *_;
    for (int x = 0; x < 6; x++) {
        spi_device_get_trans_result(*this->spi, &_, portMAX_DELAY);
    }
    this->sendingBuffer = false;
}
