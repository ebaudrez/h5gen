## tap_testname = attribute creation - two scalar attributes at (sub)group level
## h5dump_opt = -A
HDF5 "test21.h5" {
GROUP "/" {
   GROUP "Geolocation" {
      GROUP "Rectified Grid" {
         GROUP "Resolution Flags" {
            ATTRIBUTE "East West" {
               DATATYPE  H5T_IEEE_F64BE
               DATASPACE  SCALAR
               DATA {
               (0): 0.0144116
               }
            }
            ATTRIBUTE "North South" {
               DATATYPE  H5T_IEEE_F64BE
               DATASPACE  SCALAR
               DATA {
               (0): 0.0144116
               }
            }
         }
      }
   }
}
}
