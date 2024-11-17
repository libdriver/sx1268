[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SX1268

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sx1268/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SX1268サブGHz無線トランシーバーは、長距離無線アプリケーションに理想的なデバイスです。わずか4.2mAのアクティブな受信電流消費で長いバッテリ寿命を実現するように設計されています。 SX1268は、高効率の統合パワーアンプを使用して490MHzで最大+22dBmを送信できます.780MHzでは、SX1268はアンテナポートで+10dBm信号を送信するために20mA未満を消費します.SX1268はLPWANユースケースのLoRa変調をサポートします（G）レガシーユースケースのFSK変調。 LoRaWANTM標準または独自のプロトコルを利用してさまざまなアプリケーション要件を満たすように高度に構成可能デバイスは、LoRaAllianceTMによってリリースされたLoRaWANTM仕様の物理層要件に準拠するように設計されています。ただし、中国の規制要件とETSI EN 300 220（434 MHz）に限定されません。410MHzから810 MHzまでの継続的な周波数カバレッジにより、490MHzと780MHzの中国の低電力短距離デバイス帯域をサポートできます。SX1268は、スマートメーター、サプライチェーンとロジスティクス、建物の自動化など。

LibDriver SX1268は、LibDriverによって起動されたSX1268の全機能ドライバーです。これは、ワイヤレス送信、ワイヤレス受信、cadなどの機能を提供します。LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SX1268のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SX1268用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SX1268ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SX1268プログラミング例が含まれています。

/ docディレクトリには、LibDriver SX1268オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SX1268データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

#### example basic send

```C
#include "driver_sx1268_lora.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
static uint8_t gs_rx_done;

static void a_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
    switch (type)
    {
        case SX1268_IRQ_TX_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq tx done.\n");
            
            break;
        }
        case SX1268_IRQ_RX_DONE :
        {
            uint16_t i;
            sx1268_bool_t enable;
            float rssi;
            float snr;
            
            sx1268_interface_debug_print("sx1268: irq rx done.\n");
            
            /* get the status */
            if (sx1268_lora_get_status((float *)&rssi, (float *)&snr) != 0)
            {
                return;
            }
            sx1268_interface_debug_print("sx1268: rssi is %0.1f.\n", rssi);
            sx1268_interface_debug_print("sx1268: snr is %0.2f.\n", snr);
            
            /* check the error */
            if (sx1268_lora_check_packet_error(&enable) != 0)
            {
                return;
            }
            if ((enable == SX1268_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    sx1268_interface_debug_print("%c", buf[i]);
                }
                sx1268_interface_debug_print("\n");
                gs_rx_done = 1;
            }
            
            break;
        }
        case SX1268_IRQ_PREAMBLE_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq preamble detected.\n");
            
            break;
        }
        case SX1268_IRQ_SYNC_WORD_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid sync word detected.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid header.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq header error.\n");
            
            break;
        }
        case SX1268_IRQ_CRC_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq crc error.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq cad done.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq cad detected.\n");
            
            break;
        }
        case SX1268_IRQ_TIMEOUT :
        {
            sx1268_interface_debug_print("sx1268: irq timeout.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/* gpio init */
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = sx1268_lora_irq_handler;

/* lora init */
res = sx1268_lora_init(a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

/* set send mode */
res = sx1268_lora_set_send_mode();
if (res != 0)
{
    (void)sx1268_lora_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

sx1268_interface_debug_print("sx1268: send %s.\n", "123");

/* send data */
res = sx1268_lora_send((uint8_t *)"123", strlen("123"));
if (res != 0)
{
    (void)sx1268_lora_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

/* deinit */
res = sx1268_lora_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example basic receive

```c
#include "driver_sx1268_lora.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;
static uint8_t gs_rx_done;

static void a_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
    switch (type)
    {
        case SX1268_IRQ_TX_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq tx done.\n");
            
            break;
        }
        case SX1268_IRQ_RX_DONE :
        {
            uint16_t i;
            sx1268_bool_t enable;
            float rssi;
            float snr;
            
            sx1268_interface_debug_print("sx1268: irq rx done.\n");
            
            /* get the status */
            if (sx1268_lora_get_status((float *)&rssi, (float *)&snr) != 0)
            {
                return 1;
            }
            sx1268_interface_debug_print("sx1268: rssi is %0.1f.\n", rssi);
            sx1268_interface_debug_print("sx1268: snr is %0.2f.\n", snr);
            
            /* check the error */
            if (sx1268_lora_check_packet_error(&enable) != 0)
            {
                return 1;
            }
            if ((enable == SX1268_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    sx1268_interface_debug_print("%c", buf[i]);
                }
                sx1268_interface_debug_print("\n");
                gs_rx_done = 1;
            }
            
            break;
        }
        case SX1268_IRQ_PREAMBLE_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq preamble detected.\n");
            
            break;
        }
        case SX1268_IRQ_SYNC_WORD_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid sync word detected.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_VALID :
        {
            sx1268_interface_debug_print("sx1268: irq valid header.\n");
            
            break;
        }
        case SX1268_IRQ_HEADER_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq header error.\n");
            
            break;
        }
        case SX1268_IRQ_CRC_ERR :
        {
            sx1268_interface_debug_print("sx1268: irq crc error.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DONE :
        {
            sx1268_interface_debug_print("sx1268: irq cad done.\n");
            
            break;
        }
        case SX1268_IRQ_CAD_DETECTED :
        {
            sx1268_interface_debug_print("sx1268: irq cad detected.\n");
            
            break;
        }
        case SX1268_IRQ_TIMEOUT :
        {
            sx1268_interface_debug_print("sx1268: irq timeout.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/* gpio init */
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = sx1268_lora_irq_handler;

/* lora init */
res = sx1268_lora_init(a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

/* start receiving */
sx1268_interface_debug_print("sx1268: start receiving...\n");
gs_rx_done = 0;
timeout = 3000;

/* start receive */
res = sx1268_lora_set_continuous_receive_mode();
if (res != 0)
{
    (void)sx1268_lora_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

while ((timeout != 0) && (gs_rx_done == 0))
{
    timeout--;
    sx1268_interface_delay_ms(1000);
}
if (gs_rx_done == 0)
{
    /* receive timeout */
    sx1268_interface_debug_print("sx1268: receive timeout.\n");
    (void)sx1268_lora_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

/* deinit */
res = sx1268_lora_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/sx1268/index.html](https://www.libdriver.com/docs/sx1268/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。