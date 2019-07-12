/**
 * Copyright (C) 2017-2018, Decawave Limited, All Rights Reserved
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <rng/rng.h>
#include <dw1000/dw1000_mac.h>
#include <rng/nrng_encode.h>



#if MYNEWT_VAL(NRNG_VERBOSE)

void
nrng_encode(dw1000_nrng_instance_t * nrng, uint8_t seq_num, uint16_t base){

    // Workout which slots responded with a valid frames
    for (uint16_t i=0; i < 16; i++){
        if (nrng->slot_mask & 1UL << i){
            uint16_t idx = BitIndex(nrng->slot_mask, 1UL << i, SLOT_POSITION);
            nrng_frame_t * frame = nrng->frames[(base + idx)%(nrng->nframes/FRAMES_PER_RANGE)][FIRST_FRAME_IDX];
            if (frame->code == DWT_SS_TWR_NRNG_FINAL && frame->seq_num == seq_num){
                float range = dw1000_rng_tof_to_meters(dw1000_nrng_twr_to_tof_frames(nrng->parent, frame, frame));
                nrng->ranges[i] =  *(uint32_t *)&range;
            }
            else {
                nrng->ranges[i] =  0xBAAAAAD1;
            }
        }
    }

}



#endif


