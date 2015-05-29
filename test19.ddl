## tap_testname = attribute creation - one scalar attribute at root (group) level
HDF5 "test19.h5" {
GROUP "/" {
   ATTRIBUTE "Summary Thermal Products Confidence" {
      DATATYPE  H5T_IEEE_F64BE
      DATASPACE  SCALAR
      DATA {
      (0): 0.80577
      }
   }
}
}
