/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Ha Thach (tinyusb.org) for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef BLEPAIRING_H_
#define BLEPAIRING_H_

#include "bluefruit_common.h"
#include "utility/bonding.h"

#ifdef NRF_CRYPTOCELL
#include "Adafruit_nRFCrypto.h"
#endif

class BLEPairing
{
  public:
    typedef bool (*pair_passkey_cb_t ) (uint16_t conn_hdl, uint8_t const passkey[6], bool match_request);
    typedef void (*pair_complete_cb_t) (uint16_t conn_hdl, uint8_t auth_status);
    typedef void (*pair_secured_cb_t) (uint16_t conn_hdl);

    BLEPairing(void);

    bool begin(void);

    // Use static Passkey (Legacy SC)
    bool setPIN(const char* pin);

    // Set IO capacities
    void setIOCaps(bool display, bool yes_no, bool keyboard);

    // Enable/Disable Man in the middle protection
    void setMITM(bool enabled);

    // resolve address with IRK to see if it matches
    bool resolveAddress(ble_gap_addr_t const * p_addr, ble_gap_irk_t const * irk);

    //------------- Callbacks -------------//
    bool setPasskeyCallback(pair_passkey_cb_t fp);
    void setCompleteCallback(pair_complete_cb_t fp);
    void setSecuredCallback(pair_secured_cb_t fp);

    /*------------------------------------------------------------------*/
    /* INTERNAL USAGE ONLY
     * Although declare as public, it is meant to be invoked by internal
     * code. User should not call these directly
     *------------------------------------------------------------------*/
    void _eventHandler(ble_evt_t* evt);

    bool _authenticate(uint16_t conn_hdl);
    bool _encrypt(uint16_t conn_hdl, bond_keys_t const* ltkey);

  private:
    ble_gap_sec_params_t _sec_param;

    nRFCrypto_ECC_PrivateKey _private_key;

    uint8_t _pubkey[1+BLE_GAP_LESC_P256_PK_LEN];      // our public key: 1 header + 64 data
    uint8_t _peer_pubkey[1+BLE_GAP_LESC_P256_PK_LEN]; // peer public key when using LESC

    bond_keys_t  _bond_keys; // Shared keys with bonded device during securing connection, size ~ 80 bytes

    pair_passkey_cb_t  _passkey_cb;
    pair_complete_cb_t _complete_cb;
    pair_secured_cb_t  _secured_cb;
};

#endif /* BLEPAIRING_H_ */
