#ifndef CSHIELD_V2_H
#define CSHIELD_V2_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class CShield_v2 {
    public: 
        typedef enum {
            PLMN_NOT_SET = -1,
            PLMN_AUTO = 0,
            PLMN_SKT = 45005,
            PLMN_KT = 45008,
            PLMN_LGU = 45006
        } LtePlmn;

        typedef enum {
            ACK_OKAY = 0,

            // Common errors
            ACK_ERR_ARG,
            ACK_ERR_TIMEOUT,
            ACK_ERR_PARSE,
            ACK_ERR_RESP_OVERFLOW,

            // CMNG return value
            ACK_NO_MATCH,

            // AT command response
            ACK_AT_ERROR = 101,
            ACK_AT_CME_ERROR,
            ACK_AT_CMS_ERROR,

            // URC results
            ACK_URC_NONE =  0x0000,
            ACK_URC_CEREG = 0x0100,
            ACK_URC_MQTT_EVT =  0x0200,
            ACK_URC_MQTT_MSG = 0x0400,
            ACK_URC_GNSS_EVT =  0x0800,
            ACK_URC_GNSS_DATA = 0x1000
        } AckCode;

        // * 10^6 before save
        typedef struct 
        {
            int32_t gnss_service;
            int32_t gnss_status;

            int32_t latitude;
            int32_t longitude;
            int32_t altitude;
            int32_t accuracy;
            int32_t speed;
            int32_t heading;
            char datetime[32];
        } GnssData;

        typedef struct
        {
            int32_t cereg;

            char *mqtt_topic = nullptr;
            int32_t mqtt_topic_size;

            char *mqtt_msg = nullptr;
            int32_t mqtt_msg_size;

            int32_t mqtt_evt_type;
            int32_t mqtt_result;

            GnssData gnss_data;
        } UrcPacket;

        CShield_v2();

        bool begin();

        int32_t get_at_error_code();
        int32_t read_urc_pkt(UrcPacket *urc_packet);

        /**************************************** AT set command ****************************************/
        /* Mobile termination control and status commands */
        int32_t set_cfun(int32_t fun);
        int32_t set_cclk(char *time);

        /* Packet domain commands */
        int32_t set_cgdcont(int32_t cid, char *pdp_type, char *apn);
        int32_t set_cgatt(int32_t state);

        /* Network service related commands */
        int32_t set_cops(int32_t mode, int32_t format, int32_t oper);
        int32_t set_cereg(int32_t n);

        /* Mobile termination errors */
        int32_t set_cmee(int32_t n);

        /* GNSS AT commands */
        int32_t cx_gnss_control(int32_t cloud_assistance, int32_t interval, int32_t timeout);
        int32_t cx_gnss_stop();
        int32_t cx_gnss_del(int32_t mask);

        /* MQTT client AT commands */
        int32_t cx_mqtt_cfg(const char *client_id, int32_t keep_alive, int32_t clean_session);
        int32_t cx_mqtt_con(int32_t op, const char *username, const char *password, const char *url, int32_t port);
        int32_t cx_mqtt_disconnect();
        int32_t cx_mqtt_con_secure(int32_t op, const char *username, const char *password, const char *url, int32_t port, int32_t sec_tag);
        int32_t cx_mqtt_sub(const char *topic, int32_t qos);
        int32_t cx_mqtt_unsub(const char *topic);
        int32_t cx_mqtt_pub(const char *topic, const uint8_t *msg, int32_t qos, int32_t retain=0);
        int32_t cx_mqtt_pub(const char *topic, const char *msg, int32_t qos, int32_t retain=0);

        /* Socket AT commands */
        int32_t cx_socket(int32_t family, int32_t type, int32_t role, int32_t *handle, int32_t cid = 0);
        int32_t cx_close(int32_t handle, int32_t *result = nullptr);
        int32_t cx_close_all();
        int32_t cx_bind(int32_t handle, int32_t port);
        int32_t cx_connect(int32_t handle, const char *url, int32_t port, int32_t *status);
        int32_t cx_send(int32_t handle, int32_t flags, uint8_t *data, int32_t size);
        int32_t cx_send(int32_t handle, int32_t flags, char *data, int32_t size);
        int32_t cx_send_to(int32_t handle, int32_t flags, const char *url, int32_t port, uint8_t *data, int32_t size);
        int32_t cx_send_to(int32_t handle, int32_t flags, const char *url, int32_t port, char *data, int32_t size);
        
        /* RS485 custom AT commands */
        /**
         * @brief Enable RS485.
         * @param baudrate Baud rate for RS485 communication (in bps).
         * @return 0 on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_rs485_enable(int32_t baudrate);

        /**
         * @brief Disable RS485.
         * @return 0 on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_rs485_disable();

        /**
         * @brief Send data via RS485.
         * @param data_size Byte size of the data to send.
         * @param data Pointer to the data to send.
         * @return 0 on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_rs485_tx(uint8_t *data, int32_t data_size);
        int32_t cx_rs485_tx(char *data, int32_t data_size);

        /**************************************** AT read command ****************************************/
        /* General */
        int32_t get_cgmi(char *manufacturer, int32_t max_size);
        int32_t get_cgmm(char *model, int32_t max_size);
        int32_t get_cgmr(char *revision, int32_t max_size);
        int32_t get_imei(char *imei, int32_t max_size);
        int32_t get_uuid(char *uuid, int32_t max_size);

        /* Mobile termination control and status commands */
        int32_t get_cfun(int32_t *fun);
        int32_t get_cesq(int32_t *rsrq, int32_t *rsrp);
        int32_t get_band(int32_t *band); 
        int32_t get_cclk(char *time, uint8_t max_size);
        
        /* Packet domain commands */
        int32_t get_cgdcont(int32_t *cid, char *pdp_type, char *apn);
        int32_t get_cgatt(int32_t *state);
        
        /* Network service related commands */
        int32_t get_cops(int32_t *oper);
        int32_t get_cereg(int32_t *stat);

        /* Mobile termination errors */
        int32_t get_cmee(int32_t *n);

        /* UICC access commands */
        int32_t get_imsi(char *imsi, int32_t max_size);
        int32_t get_iccid(char *iccid, int32_t max_size);

        /* GNSS AT commands */
        int32_t cx_gnss_read(int32_t *gnss_service, int32_t *gnss_status);

        /* MQTT client AT commands */
        int32_t cx_get_mqtt_cfg(char *client_id, int32_t max_size, int32_t *keep_alive, int32_t *clean_session);
        int32_t cx_get_mqtt_con(int32_t *status, char *url, int32_t max_size, int32_t *port, int32_t *sec_tag);

        /* Socket AT commands */
        int32_t cx_get_socket(int32_t target_handle, int32_t *family, int32_t *role, int32_t *type);
        int32_t cx_recv(int32_t handle, int32_t flags, int32_t timeout, uint8_t *data, int32_t max_size);
        int32_t cx_recv(int32_t handle, int32_t flags, int32_t timeout, char *data, int32_t max_size);
        int32_t cx_recv_from(int32_t handle, int32_t flags, int32_t timeout, uint8_t *data, int32_t max_size);
        int32_t cx_recv_from(int32_t handle, int32_t flags, int32_t timeout, char *data, int32_t max_size);
        int32_t cx_get_addr_info(const char *hostname, char *ip_addresses, int32_t max_size, int32_t address_family = 0);

        /* RS485 custom AT commands */
        /**
         * @brief Read current RS485 baudrate.
         * @param baudrate Pointer to store current RS485 baudrate.
         * @return 0 on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_get_rs485_baudrate(int32_t *baudrate);

        /**
         * @brief Read received data from RS485.
         * @param data Pointer to the buffer to store read data.
         * @param byte_size Number of bytes to read.
         * @return read byte size on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_rs485_rx(uint8_t *data, int32_t byte_size);
        int32_t cx_rs485_rx(char *data, int32_t byte_size);

        /**
         * @brief Get the number of available bytes in the RS485 RX buffer.
         * @param byte_size Pointer to store the available byte count.
         * @return 0 on success, otherwise error code (see AckCode enum).
         */
        int32_t cx_get_rs485_rx_count(int32_t *byte_size);

        private:
        #define RESPONSE_DATA_SIZE_MAX 512
        #define AT_TX_DATA_SIZE_MAX    512
        #define AT_TX_BYTES_SIZE_MAX   256
        #define TOKEN_SIZE_MAX         32
        #define CR '\r'
        #define LF '\n'
        #define COMMA ','
        #define COLON ':'
        #define QUOTE '"'
        #define SPACE ' '
        #define NULL_TERMINATOR '\0'

        typedef enum {
            AT_OK = 0,
            AT_ERROR,
            AT_CME_ERROR,
            AT_CMS_ERROR
        } AtRespCode;

        typedef enum {
            URC_NONE = 0,
            URC_CEREG,
            URC_MQTTEVT,
            URC_XMQTTMSG,
            URC_XGNSS,
            URC_UNKNOWN
        } UrcType;
        
        typedef struct 
        {
            const char *buf;
            int32_t len;
        } AtToken;

        typedef struct 
        {
            const char *at;
            int32_t count;
            int32_t resp_code;
            int32_t at_error_code;
            AtToken tokens[TOKEN_SIZE_MAX];
        } AtParser;
        
        // HardwareSerial* _hwSerial;
        // SoftwareSerial*  _swSerial;
        // SerialType _serialType;

        char _pktbuf[RESPONSE_DATA_SIZE_MAX+8];
        char _urcbuf[RESPONSE_DATA_SIZE_MAX+8];
        AtParser _parser;
        
        const uint8_t HW_RX_PIN = 0;
        const uint8_t HW_TX_PIN = 1;
        const uint8_t SW_RX_PIN = 8;
        const uint8_t SW_TX_PIN = 7;
        
        const uint32_t BAUDRATE = 115200;   

        void uart_write(const uint8_t *data, size_t size);
        int32_t uart_read(uint8_t *buf, size_t bufsize);
        void uart_flush();
        uint32_t get_ms();
        void sleep_ms(uint32_t ms);

        
        void at_parser_init();
        static void trim_cr_lf(const char **str);
        static void trim_left_space(const char **str);
        static void trim_quote(const char **str, int32_t *len);
        static bool is_resp(const char *str);
        static bool is_end_response(const char *str);
        static bool is_end_line(const char *str);
        static bool is_colon_or_comma(const char *str);
        static bool is_quote(const char *str);
        static int32_t get_urc_type(const char *str, const char *end);
        int32_t find_prefix_token(const char *str, int32_t start_idx = 0);
        int32_t get_at_error_ack();
        bool is_empty_token(uint8_t token_index);
        
        int32_t char_to_hex(char c, uint8_t *x);
        int32_t hex_to_char(uint8_t x, char *c);
        int32_t hex_to_bin(const char *hex, size_t hexlen, uint8_t *buf, size_t buflen);
        int32_t bin_to_hex(const uint8_t *buf, int32_t buflen, char *hex, int32_t hexlen);

        static bool char_to_int32(const char *str, int32_t len, int32_t *resp);
        static bool char_to_int32(const AtToken *tok, int32_t *resp);
        int32_t transfer_pkt(const uint8_t *cmd, int32_t cmd_size, int32_t max_tokens, uint32_t timeout_ms);
        int32_t transfer_pkt(const char *cmd, int32_t max_tokens, uint32_t timeout_ms);
        int32_t at_pkt_parser(int32_t max_tokens, const uint8_t *cmd, int32_t cmd_size);

        int32_t urc_pkt_parser(UrcPacket *urc_packet, const char *end);
        int32_t parse_urc_cereg(int32_t *cereg, const char **cursor, const char *end);
        int32_t parse_urc_mqtt_evt(int32_t *evt_type, int32_t *result, const char **cursor, const char *end);
        int32_t parse_urc_mqtt_msg(char *mqtt_topic, int32_t mqtt_topic_size, 
                                char *mqtt_msg, int32_t mqtt_msg_size, const char **cursor, const char *end);
        int32_t parse_urc_gnss(GnssData *gnss_data, const char **cursor, const char *end);
};

#endif // CSHIELD_V2_H
