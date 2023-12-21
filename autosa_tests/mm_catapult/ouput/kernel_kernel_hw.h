#include "kernel_kernel.h"

struct A_IO_L2_in_local_A {
    A_t16 data[8][1];
};

struct B_IO_L2_in_local_B {
    B_t16 data[8][1];
};

struct C_drain_IO_L1_out_local_C {
    C_t4 data[8][2];
};

#include <mc_scverify.h>

/* Module Definition */
class A_IO_L3_in {
    public:
        A_IO_L3_in() {}
        #pragma hls_design interface
        #pragma hls_pipeline_init_interval 1
        void CCS_BLOCK(run)(ac_channel<A_t16> &fifo_A_in, ac_channel<A_t16> &fifo_A_local_out) {
            /* Variable Declaration */
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_A_in.available(1024))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
            for (ac_int<2, false> c3 = 0; c3 <= 1; c3 += 1)
            for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1)
#endif
            {
                // hls_pipeline
                {
                    A_t16 in_data;
                    A_t16 out_data;
                    in_data = fifo_A_in.read();
                    out_data = in_data;
                    fifo_A_local_out.write(out_data);
                }
            }
        } // END void CCS_BLOCK(run)
}; // END class A_IO_L3_in
/* Module Definition */

/* Module Definition */
class A_IO_L3_in_serialize {
    public:
        A_IO_L3_in_serialize() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(A_t16 A[1024], ac_channel<A_t16> &fifo_A_local_out) {
            /* Variable Declaration */
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
#endif
            #pragma hls_pipeline_init_interval 1
            for (ac_int<11, false> i = 0; i < 1024; i++) {
                A_t16 fifo_data;
                fifo_data = A[i];
                fifo_A_local_out.write(fifo_data);
            }
        } // END void CCS_BLOCK(run)
}; // END class A_IO_L3_in_serialize
/* Module Definition */

/* Module Definition */
class A_IO_L2_in_intra_trans {
    public:
        A_IO_L2_in_intra_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<A_IO_L2_in_local_A> &local_A, ac_channel<A_t2> &fifo_A_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (local_A.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                A_IO_L2_in_local_A local_A_tmp;
                local_A_tmp = local_A.read();
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                for (ac_int<4, false> c6 = 0; c6 <= 7; c6 += 1)
                for (ac_int<4, false> c7 = 0; c7 <= 7; c7 += 1) {
                // hls_pipeline
                {
                    A_t16 in_data;
                    A_t2 out_data;
                    A_t2 data_split[8];
                    in_data = local_A_tmp.data[c7][2 * c5 / 16];
                    data_split[0] = in_data.slc<64>(0);
                    data_split[1] = in_data.slc<64>(64);
                    data_split[2] = in_data.slc<64>(128);
                    data_split[3] = in_data.slc<64>(192);
                    data_split[4] = in_data.slc<64>(256);
                    data_split[5] = in_data.slc<64>(320);
                    data_split[6] = in_data.slc<64>(384);
                    data_split[7] = in_data.slc<64>(448);
                    int split_idx = (c5) % 8;
                    out_data = data_split[split_idx];
                    fifo_A_local_out.write(out_data);
                }
                }
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class A_IO_L2_in_inter_trans {
    public:
        A_IO_L2_in_inter_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<A_IO_L2_in_local_A> &local_A, ac_channel<A_t16> &fifo_A_in, ac_channel<A_t16> &fifo_A_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_A_in.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                A_IO_L2_in_local_A local_A_tmp;
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1) {
                    if (c3 == p0) {
                        for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                        // hls_pipeline
                        {
                            A_t16 in_data;
                            A_t16 out_data;
                            in_data = fifo_A_in.read();
                            out_data = in_data;
                            local_A_tmp.data[c4][0] = out_data;
                        }
                        }
                    } else {
                        for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                        // hls_pipeline
                        {
                            A_t16 in_data;
                            A_t16 out_data;
                            in_data = fifo_A_in.read();
                            out_data = in_data;
                            fifo_A_out.write(out_data);
                        }
                        }
                    }
                }
                local_A.write(local_A_tmp);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class A_IO_L2_in_inter_trans_boundary {
    public:
        A_IO_L2_in_inter_trans_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<A_IO_L2_in_local_A> &local_A, ac_channel<A_t16> &fifo_A_in) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_A_in.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                A_IO_L2_in_local_A local_A_tmp;
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1)
                if (c3 == p0)
                for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                    // hls_pipeline
                    {
                        A_t16 in_data;
                        A_t16 out_data;
                        in_data = fifo_A_in.read();
                        out_data = in_data;
                        local_A_tmp.data[c4][0] = out_data;
                    }
                }
                local_A.write(local_A_tmp);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class A_IO_L2_in {
    public:
        A_IO_L2_in() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<A_t16> &fifo_A_in, ac_channel<A_t16> &fifo_A_out, ac_channel<A_t2> &fifo_A_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

            A_IO_L2_in_inter_trans_inst.run(
                /* module id */ idx, 
                /* array */ A_IO_L2_in_local_A_inst, 
                /* fifo */ fifo_A_in, 
                /* fifo */ fifo_A_out
            );
            A_IO_L2_in_intra_trans_inst.run(
                /* module id */ idx, 
                /* array */ A_IO_L2_in_local_A_inst, 
                /* fifo */ fifo_A_local_out
            );
        } // END void CCS_BLOCK(run)

    private:
        A_IO_L2_in_inter_trans A_IO_L2_in_inter_trans_inst;
        A_IO_L2_in_intra_trans A_IO_L2_in_intra_trans_inst;
        ac_channel<A_IO_L2_in_local_A> A_IO_L2_in_local_A_inst;
};
/* Module Definition */

/* Module Definition */
class A_IO_L2_in_boundary {
    public:
        A_IO_L2_in_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<A_t16> &fifo_A_in, ac_channel<A_t2> &fifo_A_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

            A_IO_L2_in_inter_trans_boundary_inst.run(
                /* module id */ idx, 
                /* array */ A_IO_L2_in_local_A_inst, 
                /* fifo */ fifo_A_in
            );
            A_IO_L2_in_intra_trans_inst.run(
                /* module id */ idx, 
                /* array */ A_IO_L2_in_local_A_inst, 
                /* fifo */ fifo_A_local_out
            );
        } // END void CCS_BLOCK(run)

    private:
        A_IO_L2_in_inter_trans_boundary A_IO_L2_in_inter_trans_boundary_inst;
        A_IO_L2_in_intra_trans A_IO_L2_in_intra_trans_inst;
        ac_channel<A_IO_L2_in_local_A> A_IO_L2_in_local_A_inst;
};
/* Module Definition */

/* Module Definition */
class B_IO_L3_in {
    public:
        B_IO_L3_in() {}
        #pragma hls_design interface
        #pragma hls_pipeline_init_interval 1
        void CCS_BLOCK(run)(ac_channel<B_t16> &fifo_B_in, ac_channel<B_t16> &fifo_B_local_out) {
            /* Variable Declaration */
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_B_in.available(1024))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
            for (ac_int<2, false> c3 = 0; c3 <= 1; c3 += 1)
            for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1)
#endif
            {
                // hls_pipeline
                {
                    B_t16 in_data;
                    B_t16 out_data;
                    in_data = fifo_B_in.read();
                    out_data = in_data;
                    fifo_B_local_out.write(out_data);
                }
            }
        } // END void CCS_BLOCK(run)
}; // END class B_IO_L3_in
/* Module Definition */

/* Module Definition */
class B_IO_L3_in_serialize {
    public:
        B_IO_L3_in_serialize() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(B_t16 B[1024], ac_channel<B_t16> &fifo_B_local_out) {
            /* Variable Declaration */
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
#endif
            #pragma hls_pipeline_init_interval 1
            for (ac_int<11, false> i = 0; i < 1024; i++) {
                B_t16 fifo_data;
                fifo_data = B[i];
                fifo_B_local_out.write(fifo_data);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class B_IO_L2_in_intra_trans {
    public:
        B_IO_L2_in_intra_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<B_IO_L2_in_local_B> &local_B, ac_channel<B_t2> &fifo_B_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (local_B.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                B_IO_L2_in_local_B local_B_tmp;
                local_B_tmp = local_B.read();
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                for (ac_int<4, false> c6 = 0; c6 <= 7; c6 += 1)
                for (ac_int<4, false> c7 = 0; c7 <= 7; c7 += 1) {
                // hls_pipeline
                {
                    B_t16 in_data;
                    B_t2 out_data;
                    B_t2 data_split[8];
                    in_data = local_B_tmp.data[c6][2 * c5 / 16];
                    data_split[0] = in_data.slc<64>(0);
                    data_split[1] = in_data.slc<64>(64);
                    data_split[2] = in_data.slc<64>(128);
                    data_split[3] = in_data.slc<64>(192);
                    data_split[4] = in_data.slc<64>(256);
                    data_split[5] = in_data.slc<64>(320);
                    data_split[6] = in_data.slc<64>(384);
                    data_split[7] = in_data.slc<64>(448);
                    int split_idx = (c5) % 8;
                    out_data = data_split[split_idx];
                    fifo_B_local_out.write(out_data);
                }
                }
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class B_IO_L2_in_inter_trans {
    public:
        B_IO_L2_in_inter_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<B_IO_L2_in_local_B> &local_B, ac_channel<B_t16> &fifo_B_in, ac_channel<B_t16> &fifo_B_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_B_in.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                B_IO_L2_in_local_B local_B_tmp;
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1) {
                if (c3 == p0) {
                    for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                    // hls_pipeline
                    {
                        B_t16 in_data;
                        B_t16 out_data;
                        in_data = fifo_B_in.read();
                        out_data = in_data;
                        local_B_tmp.data[c4][0] = out_data;
                    }
                    }
                } else {
                    for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                    // hls_pipeline
                    {
                        B_t16 in_data;
                        B_t16 out_data;
                        in_data = fifo_B_in.read();
                        out_data = in_data;
                        fifo_B_out.write(out_data);
                    }
                    }
                }
                }
                local_B.write(local_B_tmp);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class B_IO_L2_in_inter_trans_boundary {
    public:
        B_IO_L2_in_inter_trans_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<B_IO_L2_in_local_B> &local_B, ac_channel<B_t16> &fifo_B_in) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_B_in.available(64))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
#endif
            {
                B_IO_L2_in_local_B local_B_tmp;
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1)
                if (c3 == p0)
                for (ac_int<4, false> c4 = 0; c4 <= 7; c4 += 1) {
                    // hls_pipeline
                    {
                        B_t16 in_data;
                        B_t16 out_data;
                        in_data = fifo_B_in.read();
                        out_data = in_data;
                        local_B_tmp.data[c4][0] = out_data;
                    }
                }
                local_B.write(local_B_tmp);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class B_IO_L2_in {
    public:
        B_IO_L2_in() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<B_t16> &fifo_B_in, ac_channel<B_t16> &fifo_B_out, ac_channel<B_t2> &fifo_B_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

            B_IO_L2_in_inter_trans_inst.run(
                /* module id */ idx, 
                /* array */ B_IO_L2_in_local_B_inst, 
                /* fifo */ fifo_B_in, 
                /* fifo */ fifo_B_out
            );

            B_IO_L2_in_intra_trans_inst.run(
                /* module id */ idx, 
                /* array */ B_IO_L2_in_local_B_inst, 
                /* fifo */ fifo_B_local_out
            );
        }

    private:
        B_IO_L2_in_inter_trans B_IO_L2_in_inter_trans_inst;
        B_IO_L2_in_intra_trans B_IO_L2_in_intra_trans_inst;
        ac_channel<B_IO_L2_in_local_B> B_IO_L2_in_local_B_inst;
};
/* Module Definition */

/* Module Definition */
class B_IO_L2_in_boundary {
    public:
        B_IO_L2_in_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<B_t16> &fifo_B_in, ac_channel<B_t2> &fifo_B_local_out) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

            B_IO_L2_in_inter_trans_boundary_inst.run(
                /* module id */ idx, 
                /* array */ B_IO_L2_in_local_B_inst, 
                /* fifo */ fifo_B_in
            );
            B_IO_L2_in_intra_trans_inst.run(
                /* module id */ idx, 
                /* array */ B_IO_L2_in_local_B_inst, 
                /* fifo */ fifo_B_local_out
            );
        }

    private:
        B_IO_L2_in_inter_trans_boundary B_IO_L2_in_inter_trans_boundary_inst;
        B_IO_L2_in_intra_trans B_IO_L2_in_intra_trans_inst;
        ac_channel<B_IO_L2_in_local_B> B_IO_L2_in_local_B_inst;
};
/* Module Definition */

/* Module Definition */
class PE {
    public:
        PE() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<A_t2> &fifo_A_in, ac_channel<A_t2> &fifo_A_out, ac_channel<B_t2> &fifo_B_in, ac_channel<B_t2> &fifo_B_out, ac_channel<C_t1> &fifo_C_drain_out) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            A_t1 local_A[1][2];
            B_t1 local_B[1][2];
            C_t1 local_C[8][8];
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_A_in.available(16) && fifo_B_in.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                #pragma hls_pipeline_init_interval 1
                for (ac_int<3, false> c2 = 0; c2 <= 3; c2 += 1)
                for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                for (ac_int<4, false> c6 = 0; c6 <= 7; c6 += 1)
                for (ac_int<4, false> c7 = 0; c7 <= 7; c7 += 1) {
                    {
                        A_t2 fifo_data;
                        fifo_data = fifo_A_in.read();
                        #pragma unroll yes
                        for (ac_int<2, false> n = 0; n < 2; n++) {
                            local_A[0][n] = (A_t1)fifo_data.slc<32>(0);
                            fifo_data = fifo_data >> 32;
                        }
                    }
                    {
                        B_t2 fifo_data;
                        fifo_data = fifo_B_in.read();
                        #pragma unroll yes
                        for (ac_int<2, false> n = 0; n < 2; n++) {
                            local_B[0][n] = (B_t1)fifo_data.slc<32>(0);
                            fifo_data = fifo_data >> 32;
                        }
                    }
                    // hls_unroll
                    {
                        C_t1 temp;
                        if (c2 == 0 && c5 == 0)
                            temp = 0;
                        for (ac_int<2, false> c8 = 0; c8 <= 1; c8 += 1){
                            temp = (local_C[c7][c6] + (local_A[0][c8] * local_B[0][c8]));
                            local_C[c7][c6] = temp;
                        }
                    }
                    if (c2 == 3 && c5 == 7)
                        fifo_C_drain_out.write(local_C[c7][c6]);
                    {
                        B_t2 fifo_data;
                        fifo_data.set_slc(32, local_B[0][1]);
                        fifo_data.set_slc(0, local_B[0][0]);
                        fifo_B_out.write(fifo_data);
                    }
                    {
                        A_t2 fifo_data;
                        fifo_data.set_slc(32, local_A[0][1]);
                        fifo_data.set_slc(0, local_A[0][0]);
                        fifo_A_out.write(fifo_data);
                    }
                }
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L1_out_intra_trans {
    public:
        C_drain_IO_L1_out_intra_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<C_drain_IO_L1_out_local_C> &local_C, ac_channel<C_t1> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            /* Variable Declaration */


#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_C_drain_local_in.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                C_drain_IO_L1_out_local_C local_C_tmp;
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<4, false> c6 = 0; c6 <= 7; c6 += 1)
                for (ac_int<4, false> c7 = 0; c7 <= 7; c7 += 1) {
                // hls_pipeline
                {
                    C_t1 in_data;
                    C_t4 out_data;
                    C_t1 data_split[4];
                    in_data = fifo_C_drain_local_in.read();
                    int split_idx = (c6) % 4;
                    out_data = local_C_tmp.data[c7][c6 / 4];
                    data_split[0] = out_data.slc<32>(0);
                    data_split[1] = out_data.slc<32>(32);
                    data_split[2] = out_data.slc<32>(64);
                    data_split[3] = out_data.slc<32>(96);
                    data_split[split_idx] = in_data;
                    out_data.set_slc(0, data_split[0]);
                    out_data.set_slc(32, data_split[1]);
                    out_data.set_slc(64, data_split[2]);
                    out_data.set_slc(96, data_split[3]);
                    local_C_tmp.data[c7][c6 / 4] = out_data;
                }
                }
                local_C.write(local_C_tmp);
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L1_out_inter_trans {
    public:
        C_drain_IO_L1_out_inter_trans() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<C_drain_IO_L1_out_local_C> &local_C, ac_channel<C_t4> &fifo_C_drain_in, ac_channel<C_t4> &fifo_C_drain_out) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (local_C.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                C_drain_IO_L1_out_local_C local_C_tmp;
                local_C_tmp = local_C.read();
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c4 = p1; c4 <= 1; c4 += 1) {
                    if (c4 == p1) {
                        for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                        for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                        // hls_pipeline
                        {
                            C_t4 in_data;
                            C_t4 out_data;
                            in_data = local_C_tmp.data[c5][c6];
                            out_data = in_data;
                            fifo_C_drain_out.write(out_data);
                        }
                        }
                    } 
                    else {
                        for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                        for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                        // hls_pipeline
                        {
                            C_t4 in_data;
                            C_t4 out_data;
                            in_data = fifo_C_drain_in.read();
                            out_data = in_data;
                            fifo_C_drain_out.write(out_data);
                        }
                        }
                    }
                } // END for (ac_int<2, false> c4 = p1; c4 <= 1; c4 += 1)
            } // END for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
        } // END void CCS_BLOCK(run)
}; // END class C_drain_IO_L1_out_inter_trans
/* Module Definition */

/* Module Definition */
class C_drain_IO_L1_out_inter_trans_boundary {
    public:
        C_drain_IO_L1_out_inter_trans_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<C_drain_IO_L1_out_local_C> &local_C, ac_channel<C_t4> &fifo_C_drain_out) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (local_C.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                C_drain_IO_L1_out_local_C local_C_tmp;
                local_C_tmp = local_C.read();
                // synth
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c4 = p1; c4 <= 1; c4 += 1)
                if (c4 == p1)
                for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                // hls_pipeline
                {
                    C_t4 in_data;
                    C_t4 out_data;
                    in_data = local_C_tmp.data[c5][c6];
                    out_data = in_data;
                    fifo_C_drain_out.write(out_data);
                }
                }
            }
        } // END void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L1_out {
    public:
        C_drain_IO_L1_out() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<C_t4> &fifo_C_drain_in, ac_channel<C_t4> &fifo_C_drain_out, ac_channel<C_t1> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            /* Variable Declaration */

            C_drain_IO_L1_out_intra_trans_inst.run(
                /* module id */ idx, 
                /* module id */ idy, 
                /* array */ C_drain_IO_L1_out_local_C_inst, 
                /* fifo */ fifo_C_drain_local_in
            );
            
            C_drain_IO_L1_out_inter_trans_inst.run(
                /* module id */ idx, 
                /* module id */ idy, 
                /* array */ C_drain_IO_L1_out_local_C_inst, 
                /* fifo */ fifo_C_drain_in, 
                /* fifo */ fifo_C_drain_out
            );
        } // End void CCS_BLOCK(run)

    private:
        C_drain_IO_L1_out_inter_trans C_drain_IO_L1_out_inter_trans_inst;
        C_drain_IO_L1_out_intra_trans C_drain_IO_L1_out_intra_trans_inst;
        ac_channel<C_drain_IO_L1_out_local_C> C_drain_IO_L1_out_local_C_inst;
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L1_out_boundary {
    public:
        C_drain_IO_L1_out_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, int idy, ac_channel<C_t4> &fifo_C_drain_out, ac_channel<C_t1> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            int p0 = idx, p1 = idy; // module id
            /* Variable Declaration */

            C_drain_IO_L1_out_intra_trans_inst.run(
                /* module id */ idx, 
                /* module id */ idy, 
                /* array */ C_drain_IO_L1_out_local_C_inst, 
                /* fifo */ fifo_C_drain_local_in
            );
            C_drain_IO_L1_out_inter_trans_boundary_inst.run(
                /* module id */ idx, 
                /* module id */ idy, 
                /* array */ C_drain_IO_L1_out_local_C_inst, 
                /* fifo */ fifo_C_drain_out
            );
        } // End void CCS_BLOCK(run)

    private:
        C_drain_IO_L1_out_inter_trans_boundary C_drain_IO_L1_out_inter_trans_boundary_inst;
        C_drain_IO_L1_out_intra_trans C_drain_IO_L1_out_intra_trans_inst;
        ac_channel<C_drain_IO_L1_out_local_C> C_drain_IO_L1_out_local_C_inst;
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L2_out {
    public:
        C_drain_IO_L2_out() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<C_t4> &fifo_C_drain_in, ac_channel<C_t4> &fifo_C_drain_out, ac_channel<C_t4> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_C_drain_local_in.available(16) || fifo_C_drain_in.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1) {
                    if (c3 == p0) {
                        for (ac_int<2, false> c4 = 0; c4 <= 1; c4 += 1)
                        for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                        for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                            // hls_pipeline
                            {
                                C_t4 in_data;
                                C_t4 out_data;
                                in_data = fifo_C_drain_local_in.read();
                                out_data = in_data;
                                fifo_C_drain_out.write(out_data);
                            }
                        }
                    } else {
                        for (ac_int<2, false> c4 = 0; c4 <= 1; c4 += 1)
                        for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                        for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                            // hls_pipeline
                            {
                                C_t4 in_data;
                                C_t4 out_data;
                                in_data = fifo_C_drain_in.read();
                                out_data = in_data;
                                fifo_C_drain_out.write(out_data);
                            }
                        }
                    }
                }
            }
        } // End void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L2_out_boundary {
    public:
        C_drain_IO_L2_out_boundary() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(int idx, ac_channel<C_t4> &fifo_C_drain_out, ac_channel<C_t4> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            int p0 = idx; // module id
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_C_drain_local_in.available(16))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
#endif
            {
                #pragma hls_pipeline_init_interval 1
                for (ac_int<2, false> c3 = p0; c3 <= 1; c3 += 1)
                if (c3 == p0)
                for (ac_int<2, false> c4 = 0; c4 <= 1; c4 += 1)
                for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
                for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1) {
                    // hls_pipeline
                    {
                        C_t4 in_data;
                        C_t4 out_data;
                        in_data = fifo_C_drain_local_in.read();
                        out_data = in_data;
                        fifo_C_drain_out.write(out_data);
                    }
                }
            }
        } // End void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L3_out {
    public:
        C_drain_IO_L3_out() {}
        #pragma hls_design interface
        #pragma hls_pipeline_init_interval 1
        void CCS_BLOCK(run)(ac_channel<C_t4> &fifo_C_drain_out, ac_channel<C_t4> &fifo_C_drain_local_in) {
            /* Variable Declaration */
            /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_C_drain_local_in.available(1024))
            for (ac_int<3, false> c0 = 0; c0 <= 3; c0 += 1)
            for (ac_int<3, false> c1 = 0; c1 <= 3; c1 += 1)
            for (ac_int<2, false> c3 = 0; c3 <= 1; c3 += 1)
            for (ac_int<2, false> c4 = 0; c4 <= 1; c4 += 1)
            for (ac_int<4, false> c5 = 0; c5 <= 7; c5 += 1)
            for (ac_int<2, false> c6 = 0; c6 <= 1; c6 += 1)
#endif
            {
                // hls_pipeline
                {
                    C_t4 in_data;
                    C_t4 out_data;
                    in_data = fifo_C_drain_local_in.read();
                    out_data = in_data;
                    fifo_C_drain_out.write(out_data);
                }
            }
        } // End void CCS_BLOCK(run)
};
/* Module Definition */

/* Module Definition */
class C_drain_IO_L3_out_serialize {
    public:
        C_drain_IO_L3_out_serialize() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(C_t16 C[256], ac_channel<C_t4> &fifo_C_drain_local_in) {
        /* Variable Declaration */
        /* Variable Declaration */

#ifndef __SYNTHESIS__
            // while () // Please add the fifo check for C sim.
            while (fifo_C_drain_local_in.available(1))
#endif
            #pragma hls_pipeline_init_interval 1
            for (ac_int<9, false> i = 0; i < 256; i++) {
                C_t4 fifo_data;
                C_t16 mem_data;
                C_t4 mem_data_split[4];
                for (ac_int<3, false> p = 0; p < 4; p++) {
                    fifo_data = fifo_C_drain_local_in.read();
                    mem_data_split[p] = fifo_data;
                }
                mem_data.set_slc(0, mem_data_split[0]);
                mem_data.set_slc(128, mem_data_split[1]);
                mem_data.set_slc(256, mem_data_split[2]);
                mem_data.set_slc(384, mem_data_split[3]);
                C[i] = mem_data;
            }
        } // End void CCS_BLOCK(run)
};
/* Module Definition */

#pragma hls_design top
class kernel0 {
    public:
        kernel0() {}
        #pragma hls_design interface
        void CCS_BLOCK(run)(A_t16 A[16384 / 16], B_t16 B[16384 / 16], C_t16 C[4096 / 16]) {
            /* Module Call */
            A_IO_L3_in_serialize_inst.run(
                /* array */ A,
                /* fifo */ fifo_A_A_IO_L3_in_serialize
            );
            /* Module Call */

            /* Module Call */
            A_IO_L3_in_inst.run(
                /* fifo */ fifo_A_A_IO_L3_in_serialize,
                /* fifo */ fifo_A_A_IO_L2_in_0
            );
            /* Module Call */

            /* Module Call */
            A_IO_L2_in_inst_0.run(
                /* module id */ 0,
                /* fifo */ fifo_A_A_IO_L2_in_0,
                /* fifo */ fifo_A_A_IO_L2_in_1,
                /* fifo */ fifo_A_PE_0_0
            );
            /* Module Call */

            /* Module Call */
            A_IO_L2_in_boundary_inst_1.run(
                /* module id */ 1,
                /* fifo */ fifo_A_A_IO_L2_in_1,
                /* fifo */ fifo_A_PE_1_0
            );
            /* Module Call */

            /* Module Call */
            B_IO_L3_in_serialize_inst.run(
                /* array */ B,
                /* fifo */ fifo_B_B_IO_L3_in_serialize
            );
            /* Module Call */

            /* Module Call */
            B_IO_L3_in_inst.run(
                /* fifo */ fifo_B_B_IO_L3_in_serialize,
                /* fifo */ fifo_B_B_IO_L2_in_0
            );
            /* Module Call */

            /* Module Call */
            B_IO_L2_in_inst_0.run(
                /* module id */ 0,
                /* fifo */ fifo_B_B_IO_L2_in_0,
                /* fifo */ fifo_B_B_IO_L2_in_1,
                /* fifo */ fifo_B_PE_0_0
            );
            /* Module Call */

            /* Module Call */
            B_IO_L2_in_boundary_inst_1.run(
                /* module id */ 1,
                /* fifo */ fifo_B_B_IO_L2_in_1,
                /* fifo */ fifo_B_PE_0_1
            );
            /* Module Call */

            /* Module Call */
            PE_inst_0_0.run(
                /* module id */ 0,
                /* module id */ 0,
                /* fifo */ fifo_A_PE_0_0,
                /* fifo */ fifo_A_PE_0_1,
                /* fifo */ fifo_B_PE_0_0,
                /* fifo */ fifo_B_PE_1_0,
                /* fifo */ fifo_C_drain_PE_0_0
            );
            /* Module Call */

            /* Module Call */
            PE_inst_0_1.run(
                /* module id */ 0,
                /* module id */ 1,
                /* fifo */ fifo_A_PE_0_1,
                /* fifo */ fifo_A_PE_0_2,
                /* fifo */ fifo_B_PE_0_1,
                /* fifo */ fifo_B_PE_1_1,
                /* fifo */ fifo_C_drain_PE_0_1
            );
            /* Module Call */

            /* Module Call */
            PE_inst_1_0.run(
                /* module id */ 1,
                /* module id */ 0,
                /* fifo */ fifo_A_PE_1_0,
                /* fifo */ fifo_A_PE_1_1,
                /* fifo */ fifo_B_PE_1_0,
                /* fifo */ fifo_B_PE_2_0,
                /* fifo */ fifo_C_drain_PE_1_0
            );
            /* Module Call */

            /* Module Call */
            PE_inst_1_1.run(
                /* module id */ 1,
                /* module id */ 1,
                /* fifo */ fifo_A_PE_1_1,
                /* fifo */ fifo_A_PE_1_2,
                /* fifo */ fifo_B_PE_1_1,
                /* fifo */ fifo_B_PE_2_1,
                /* fifo */ fifo_C_drain_PE_1_1
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L1_out_boundary_inst_0_1.run(
                /* module id */ 0,
                /* module id */ 1,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_0_1,
                /* fifo */ fifo_C_drain_PE_1_0
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L1_out_inst_0_0.run(
                /* module id */ 0,
                /* module id */ 0,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_0_1,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_0_0,
                /* fifo */ fifo_C_drain_PE_0_0
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L1_out_boundary_inst_1_1.run(
                /* module id */ 1,
                /* module id */ 1,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_1_1,
                /* fifo */ fifo_C_drain_PE_1_1
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L1_out_inst_1_0.run(
                /* module id */ 1,
                /* module id */ 0,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_1_1,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_1_0,
                /* fifo */ fifo_C_drain_PE_0_1
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L2_out_boundary_inst_1.run(
                /* module id */ 1,
                /* fifo */ fifo_C_drain_C_drain_IO_L2_out_1,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_1_0
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L2_out_inst_0.run(
                /* module id */ 0,
                /* fifo */ fifo_C_drain_C_drain_IO_L2_out_1,
                /* fifo */ fifo_C_drain_C_drain_IO_L2_out_0,
                /* fifo */ fifo_C_drain_C_drain_IO_L1_out_0_0
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L3_out_inst.run(
                /* fifo */ fifo_C_drain_C_drain_IO_L3_out_serialize,
                /* fifo */ fifo_C_drain_C_drain_IO_L2_out_0
            );
            /* Module Call */

            /* Module Call */
            C_drain_IO_L3_out_serialize_inst.run(
                /* array */ C,
                /* fifo */ fifo_C_drain_C_drain_IO_L3_out_serialize
            );
            /* Module Call */

        }

    private:
        /* Module Declaration */
        A_IO_L3_in_serialize A_IO_L3_in_serialize_inst;
        A_IO_L3_in A_IO_L3_in_inst;
        A_IO_L2_in A_IO_L2_in_inst_0;
        A_IO_L2_in_boundary A_IO_L2_in_boundary_inst_1;
        B_IO_L3_in_serialize B_IO_L3_in_serialize_inst;
        B_IO_L3_in B_IO_L3_in_inst;
        B_IO_L2_in B_IO_L2_in_inst_0;
        B_IO_L2_in_boundary B_IO_L2_in_boundary_inst_1;
        PE PE_inst_0_0;
        PE PE_inst_0_1;
        PE PE_inst_1_0;
        PE PE_inst_1_1;
        C_drain_IO_L1_out C_drain_IO_L1_out_inst_0_0;
        C_drain_IO_L1_out_boundary C_drain_IO_L1_out_boundary_inst_0_1;
        C_drain_IO_L1_out C_drain_IO_L1_out_inst_1_0;
        C_drain_IO_L1_out_boundary C_drain_IO_L1_out_boundary_inst_1_1;
        C_drain_IO_L2_out C_drain_IO_L2_out_inst_0;
        C_drain_IO_L2_out_boundary C_drain_IO_L2_out_boundary_inst_1;
        C_drain_IO_L3_out C_drain_IO_L3_out_inst;
        C_drain_IO_L3_out_serialize C_drain_IO_L3_out_serialize_inst;
        /* Module Declaration */

        /* FIFO Declaration */
        /* A_IO_L3_in_serialize fifo */ ac_channel<A_t16> fifo_A_A_IO_L3_in_serialize;
        /* B_IO_L3_in_serialize fifo */ ac_channel<B_t16> fifo_B_B_IO_L3_in_serialize;
        /* C_drain_IO_L3_out_serialize fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L3_out_serialize;
        /* A_IO_L2_in fifo */ ac_channel<A_t16> fifo_A_A_IO_L2_in_0;
        /* A_IO_L2_in fifo */ ac_channel<A_t16> fifo_A_A_IO_L2_in_1;
        /* A_IO_L2_in fifo */ ac_channel<A_t16> fifo_A_A_IO_L2_in_2;
        /* B_IO_L2_in fifo */ ac_channel<B_t16> fifo_B_B_IO_L2_in_0;
        /* B_IO_L2_in fifo */ ac_channel<B_t16> fifo_B_B_IO_L2_in_1;
        /* B_IO_L2_in fifo */ ac_channel<B_t16> fifo_B_B_IO_L2_in_2;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_0_0;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_0_1;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_0_2;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_1_0;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_1_1;
        /* PE fifo */ ac_channel<A_t2> fifo_A_PE_1_2;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_0_0;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_1_0;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_2_0;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_0_1;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_1_1;
        /* PE fifo */ ac_channel<B_t2> fifo_B_PE_2_1;
        /* PE fifo */ ac_channel<C_t1> fifo_C_drain_PE_0_0;
        /* PE fifo */ ac_channel<C_t1> fifo_C_drain_PE_1_0;
        /* PE fifo */ ac_channel<C_t1> fifo_C_drain_PE_0_1;
        /* PE fifo */ ac_channel<C_t1> fifo_C_drain_PE_1_1;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_0_0;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_0_1;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_0_2;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_1_0;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_1_1;
        /* C_drain_IO_L1_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L1_out_1_2;
        /* C_drain_IO_L2_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L2_out_0;
        /* C_drain_IO_L2_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L2_out_1;
        /* C_drain_IO_L2_out fifo */ ac_channel<C_t4> fifo_C_drain_C_drain_IO_L2_out_2;
        /* FIFO Declaration */
};
