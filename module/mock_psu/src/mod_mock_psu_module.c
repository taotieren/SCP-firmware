/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2018, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <fwk_assert.h>
#include <fwk_macros.h>
#include <fwk_mm.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <mod_mock_psu.h>
#include <mod_mock_psu_private.h>

static struct mod_mock_psu_device_ctx (*device_ctx)[];

static struct mod_mock_psu_device_ctx *get_device_ctx(
    fwk_id_t device_id)
{
    return &(*device_ctx)[fwk_id_get_element_idx(device_id)];
}

static int mock_psu_init(
    fwk_id_t module_id,
    unsigned int element_count,
    const void *data)
{
    device_ctx = fwk_mm_calloc(
        element_count,
        sizeof((*device_ctx)[0]));
    if (device_ctx == NULL)
        return FWK_E_NOMEM;

    return FWK_SUCCESS;
}

static int mock_psu_element_init(
    fwk_id_t device_id,
    unsigned int sub_element_count,
    const void *data)
{
    struct mod_mock_psu_device_ctx *ctx;
    const struct mod_mock_psu_device_config *config = data;

    assert(sub_element_count == 0);

    ctx = get_device_ctx(device_id);
    ctx->enabled = config->default_enabled;
    ctx->voltage = config->default_voltage;

    return FWK_SUCCESS;
}

static int mock_psu_process_bind_request(
    fwk_id_t source_id,
    fwk_id_t target_id,
    fwk_id_t api_id,
    const void **api)
{
    /* Only accept binds to the elements */
    if (!fwk_id_is_type(target_id, FWK_ID_TYPE_ELEMENT))
        return FWK_E_PARAM;

    *api = &__mod_mock_psu_psu_driver_api;

    return FWK_SUCCESS;
}

struct mod_mock_psu_device_ctx *__mod_mock_psu_get_valid_device_ctx(
    fwk_id_t device_id)
{
    if (fwk_module_check_call(device_id) != FWK_SUCCESS)
        return NULL;

    return get_device_ctx(device_id);
}

/* Module description */
const struct fwk_module module_mock_psu = {
    .name = "MOCK_PSU",
    .type = FWK_MODULE_TYPE_DRIVER,
    .api_count = MOD_MOCK_PSU_API_COUNT,
    .init = mock_psu_init,
    .element_init = mock_psu_element_init,
    .process_bind_request = mock_psu_process_bind_request,
};
