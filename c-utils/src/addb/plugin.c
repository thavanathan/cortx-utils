/*
 * Copyright (c) 2020 Seagate Technology LLC and/or its Affiliates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For any questions about this software or licensing,
 * please email opensource@seagate.com or cortx-questions@seagate.com.
 *
 */

#include <stdio.h>
#include <addb2/plugin_api.h>
#include <operation.h>
#include "perf/perf-counters.h"

/* Borrowed from addb2/dump.c, hope Motr will publish it as API in future */

static void dec(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "%" PRId64, v[0]);
}

static void hex(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "%" PRIx64, v[0]);
}

static void hex0x(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "0x%" PRIx64, v[0]);
}

static void oct(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "%" PRIo64, v[0]);
}

static void ptr(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "@%p", *(void **)v);
}

static void bol(struct m0_addb2__context *ctx, const uint64_t *v, char *buf) {
  sprintf(buf, "%s", v[0] ? "true" : "false");
}

/* end of clip from dump.c */

static void perfc_entity_states(struct m0_addb2__context *ctx, const uint64_t *v,
                         char *buf) {
  if (v[0] == PES_GEN_INIT)
    sprintf(buf, "%s", "init");
  if (v[0] == PES_GEN_FINI)
    sprintf(buf, "%s", "fini");
}

static void perfc_entity_attrs(struct m0_addb2__context *ctx, const uint64_t *v,
                         	 char *buf) {
  if (v[0] == PEA_R_OFFSET)
    sprintf(buf, "%s", "read_offset");
  if (v[0] == PEA_R_IOVC)
    sprintf(buf, "%s", "read_vector_count");
  if (v[0] == PEA_R_IOVL)
    sprintf(buf, "%s", "read_length");
  if (v[0] == PEA_R_RES_MAJ)
    sprintf(buf, "%s", "read_result_major_code");
  if (v[0] == PEA_R_RES_MIN)
    sprintf(buf, "%s", "read_result_minor_code");


  if (v[0] == PEA_W_OFFSET)
    sprintf(buf, "%s", "write_offset");
  if (v[0] == PEA_W_SIZE)
    sprintf(buf, "%s", "write_size");
  if (v[0] == PEA_W_RES_MAJ)
    sprintf(buf, "%s", "write_result_major_code");
  if (v[0] == PEA_W_RES_MIN)
    sprintf(buf, "%s", "write_result_minor_code");
}

static void perfc_entity_maps(struct m0_addb2__context *ctx, const uint64_t *v,
                         char *buf) {
  if (v[0] == PEM_NFS_TO_CFS)
    sprintf(buf, "%s", "NFS_TO_CFS");
  if (v[0] == PEM_NFS_TO_DSAL)
    sprintf(buf, "%s", "NFS_TO_DSAL");
  if (v[0] == PEM_NFS_TO_NSAL)
    sprintf(buf, "%s", "NFS_TO_NSAL");
  if (v[0] == PEM_NFS_TO_MOTR)
    sprintf(buf, "%s", "NFS_TO_MOTR");


  if (v[0] == PEM_CFS_TO_NFS)
    sprintf(buf, "%s", "CFS_TO_NFS");
  if (v[0] == PEM_CFS_TO_DSAL)
    sprintf(buf, "%s", "CFS_TO_DSAL");
  if (v[0] == PEM_CFS_TO_NSAL)
    sprintf(buf, "%s", "CFS_TO_NSAL");
  if (v[0] == PEM_CFS_TO_MOTR)
    sprintf(buf, "%s", "CFS_TO_MOTR");


  if (v[0] == PEM_DSAL_TO_NFS)
    sprintf(buf, "%s", "DSAL_TO_NFS");
  if (v[0] == PEM_DSAL_TO_CFS)
    sprintf(buf, "%s", "DSAL_TO_CFS");
  if (v[0] == PEM_DSAL_TO_MOTR)
    sprintf(buf, "%s", "DSAL_TO_MOTR");


  if (v[0] == PEM_NSAL_TO_NFS)
    sprintf(buf, "%s", "NSAL_TO_NFS");
  if (v[0] == PEM_NSAL_TO_CFS)
    sprintf(buf, "%s", "NSAL_TO_CFS");
  if (v[0] == PEM_NSAL_TO_MOTR)
    sprintf(buf, "%s", "NSAL_TO_MOTR");

}

static void perfc_function_tags(struct m0_addb2__context *ctx, const uint64_t *v,
                         char *buf) {
  if (v[0] == PFT_FSAL_WRITE)
    sprintf(buf, "%s", "fsal_write");
  if (v[0] == PFT_FSAL_READ)
    sprintf(buf, "%s", "fsal_read");
  if (v[0] == PFT_CFS_WRITE)
    sprintf(buf, "%s", "cfs_write");
  if (v[0] == PFT_CFS_READ)
    sprintf(buf, "%s", "cfs_read");
}

static void perfc_entry_type(struct m0_addb2__context *ctx, const uint64_t *v,
                         char *buf) {
  if (v[0] == PET_STATE)
    sprintf(buf, "%s", "state");
  if (v[0] == PET_ATTR)
    sprintf(buf, "%s", "attribute");
  if (v[0] == PET_MAP)
    sprintf(buf, "%s", "map");
}

static struct m0_addb2__id_intrp gs_curr_ids[] = {
    {
        TSDB_MK_AID(TSDB_MOD_FSUSER, 0xAB),
        "fsuser_state",
        {
            &perfc_function_tags,
			&perfc_entry_type,
			&hex, // operation id
			&perfc_entity_states
        }
    },
    {
        TSDB_MK_AID(TSDB_MOD_FSUSER, 0xCD),
        "fsuser_attribute",
        {
            &perfc_function_tags,
			&perfc_entry_type,
			&hex, // operation id
			&perfc_entity_attrs
        }
    },
    {
        TSDB_MK_AID(TSDB_MOD_FSUSER, 0xEF),
        "fsuser_map",
        {
            &perfc_function_tags,
			&perfc_entry_type,
			&hex, // operation id
			&perfc_entity_maps
        }
    },

    {0}
};

int m0_addb2_load_interps(uint64_t flags,
                          struct m0_addb2__id_intrp **intrps_array) {
  /* suppres "unused" warnings */
  (void)dec;
  (void)hex0x;
  (void)oct;
  (void)hex;
  (void)bol;
  (void)ptr;

  *intrps_array = gs_curr_ids;
  return 0;
}
