#include <gbdk/platform.h>
#include <stdint.h>
#include <string.h>

#include "states.h"
#include "bankdata.h"

static uint8_t _save;

uint8_t call_far(const far_ptr_t *ptr) NONBANKED NAKED {
    ptr;
#if defined(NINTENDO)
__asm
        ld h, d
        ld l, e
        ld a, (hl+)
        ld e, a         ; e = ptr->SEG
        ld a, (hl+)
        ld h, (hl)
        ld l, a         ; hl = ptr->SEG
        or h
        ret z           ; return if zero pointer
        jp ___sdcc_bcall_ehl
__endasm;
#endif
}

uint8_t * banked_strcpy(uint8_t *dest, const uint8_t *src, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    uint8_t * res = strcpy(dest, src);
    SWITCH_ROM(_save);
    return res;
}

void * banked_memcpy(void *dest, const void *src, size_t len, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    void * res = memcpy(dest, src, len);
    SWITCH_ROM(_save);
    return res;
}

void set_banked_sprite_data(uint8_t first_tile, uint8_t nb_tiles, const uint8_t *data, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    set_sprite_data(first_tile, nb_tiles, data);
    SWITCH_ROM(_save);
}

void set_banked_bkg_data(uint8_t first_tile, uint8_t nb_tiles, const uint8_t *data, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    set_bkg_data(first_tile, nb_tiles, data);
    SWITCH_ROM(_save);
}

void set_banked_bkg_submap(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *map, uint8_t map_w, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    set_bkg_submap(x, y, w, h, map, map_w);
    SWITCH_ROM(_save);
}

uint8_t read_banked_ubyte(const uint8_t * ptr, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    uint8_t res = *ptr;
    SWITCH_ROM(_save);
    return res;
}

void set_banked_data(uint8_t *vram_addr, const uint8_t *data, uint16_t len, uint8_t bank) NONBANKED {
    _save = _current_bank;
    SWITCH_ROM(bank);
    set_data(vram_addr, data, len);
    SWITCH_ROM(_save);
}
