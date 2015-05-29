## tap_testname = attribute creation - two scalar attributes at dataset level
## h5dump_opt = -A
HDF5 "test22.h5" {
GROUP "/" {
   GROUP "Radiometry" {
      DATASET "Longwave Correction" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.005
            }
         }
         ATTRIBUTE "Offset" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 1
            }
         }
      }
   }
}
}
