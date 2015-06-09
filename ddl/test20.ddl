## tap_testname = attribute creation - one scalar attribute at dataset level
## h5dump_opt = -A
HDF5 "test20.h5" {
GROUP "/" {
   GROUP "Angles" {
      DATASET "Relative Azimuth" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.1
            }
         }
      }
   }
}
}
