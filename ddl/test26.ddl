## tap_testname = DDL for a real-life example (GERB-2 HR) (no data)
## h5dump_opt = -A
# real name was "G2_SEV1_L20_HR_SOL_TH_20040701_120000_V003.hdf"
HDF5 "test26.h5" {
GROUP "/" {
   ATTRIBUTE "File Creation Time" {
      DATATYPE  H5T_STRING {
         STRSIZE 18;
         STRPAD H5T_STR_NULLTERM;
         CSET H5T_CSET_ASCII;
         CTYPE H5T_C_S1;
      }
      DATASPACE  SCALAR
      DATA {
      (0): "20070319 18:13:46"
      }
   }
   ATTRIBUTE "File Name" {
      DATATYPE  H5T_STRING {
         STRSIZE 47;
         STRPAD H5T_STR_NULLTERM;
         CSET H5T_CSET_ASCII;
         CTYPE H5T_C_S1;
      }
      DATASPACE  SCALAR
      DATA {
      (0): "G2_SEV1_L20_HR_SOL_TH_20040701_120000_V003.hdf"
      }
   }
   ATTRIBUTE "Summary Solar Products Confidence" {
      DATATYPE  H5T_IEEE_F64BE
      DATASPACE  SCALAR
      DATA {
      (0): 0.915634
      }
   }
   ATTRIBUTE "Summary Thermal Products Confidence" {
      DATATYPE  H5T_IEEE_F64BE
      DATASPACE  SCALAR
      DATA {
      (0): 0.80577
      }
   }
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
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 7;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Degree"
            }
         }
      }
      DATASET "Solar Zenith" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.1
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 7;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Degree"
            }
         }
      }
      DATASET "Viewing Azimuth" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.1
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 7;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Degree"
            }
         }
      }
      DATASET "Viewing Zenith" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.1
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 7;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Degree"
            }
         }
      }
   }
   GROUP "GERB" {
      ATTRIBUTE "Instrument Identifier" {
         DATATYPE  H5T_STRING {
            STRSIZE 3;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "G2"
         }
      }
   }
   GROUP "GGSPS" {
      ATTRIBUTE "L1.5 NANRG Product Version" {
         DATATYPE  H5T_STD_I32BE
         DATASPACE  SCALAR
         DATA {
         (0): -1
         }
      }
   }
   GROUP "Geolocation" {
      ATTRIBUTE "Geolocation File Name" {
         DATATYPE  H5T_STRING {
            STRSIZE 44;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "G2_SEV1_L20_HR_GEO_20060331_023000_V003.hdf"
         }
      }
      ATTRIBUTE "Line of Sight North-South Speed" {
         DATATYPE  H5T_IEEE_F64BE
         DATASPACE  SCALAR
         DATA {
         (0): 0
         }
      }
      ATTRIBUTE "Nominal Satellite Longitude (degrees)" {
         DATATYPE  H5T_IEEE_F64BE
         DATASPACE  SCALAR
         DATA {
         (0): -3.4
         }
      }
      GROUP "Rectified Grid" {
         ATTRIBUTE "Grid Orientation" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0
            }
         }
         ATTRIBUTE "Lap" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0
            }
         }
         ATTRIBUTE "Lop" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0
            }
         }
         ATTRIBUTE "Nr" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 6.61067
            }
         }
         ATTRIBUTE "Nx" {
            DATATYPE  H5T_STD_I32BE
            DATASPACE  SCALAR
            DATA {
            (0): 1237
            }
         }
         ATTRIBUTE "Ny" {
            DATATYPE  H5T_STD_I32BE
            DATASPACE  SCALAR
            DATA {
            (0): 1237
            }
         }
         ATTRIBUTE "Xp" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 618.333
            }
         }
         ATTRIBUTE "Yp" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 617.667
            }
         }
         ATTRIBUTE "dx" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 1207.44
            }
         }
         ATTRIBUTE "dy" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 1203.32
            }
         }
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
   GROUP "Imager" {
      ATTRIBUTE "Instrument Identifier" {
         DATATYPE  H5T_STD_I32BE
         DATASPACE  SCALAR
         DATA {
         (0): 1
         }
      }
      ATTRIBUTE "Type" {
         DATATYPE  H5T_STRING {
            STRSIZE 7;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "SEVIRI"
         }
      }
   }
   GROUP "RMIB" {
      ATTRIBUTE "Product Version" {
         DATATYPE  H5T_STD_I32BE
         DATASPACE  SCALAR
         DATA {
         (0): 3
         }
      }
      ATTRIBUTE "Software Identifier" {
         DATATYPE  H5T_STRING {
            STRSIZE 16;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "20070305_144906"
         }
      }
   }
   GROUP "Radiometry" {
      DATASET "A Values (per GERB detector cell)" {
         DATATYPE  H5T_IEEE_F64BE
         DATASPACE  SIMPLE { ( 256 ) / ( 256 ) }
      }
      DATASET "C Values (per GERB detector cell)" {
         DATATYPE  H5T_IEEE_F64BE
         DATASPACE  SIMPLE { ( 256 ) / ( 256 ) }
      }
      DATASET "Longwave Correction" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Offset" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 1
            }
         }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.005
            }
         }
      }
      DATASET "Shortwave Correction" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Offset" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 1
            }
         }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.005
            }
         }
      }
      DATASET "Solar Flux" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.25
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 22;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Watt per square meter"
            }
         }
      }
      DATASET "Solar Radiance" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.05
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 36;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Watt per square meter per steradian"
            }
         }
      }
      GROUP "Spectral Regression Parameters" {
         DATASET "Longwave Solar" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 10, 11 ) / ( 10, 11 ) }
         }
         DATASET "Longwave Thermal" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 18, 37 ) / ( 18, 37 ) }
         }
         DATASET "Shortwave Solar" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 10, 11 ) / ( 10, 11 ) }
         }
         DATASET "Shortwave Thermal" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 18, 37 ) / ( 18, 37 ) }
         }
         DATASET "Solar" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 10, 11 ) / ( 10, 11 ) }
         }
         DATASET "Thermal" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SIMPLE { ( 19, 37 ) / ( 19, 37 ) }
         }
      }
      DATASET "Thermal Flux" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.25
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 22;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Watt per square meter"
            }
         }
      }
      DATASET "Thermal Radiance" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.05
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 36;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Watt per square meter per steradian"
            }
         }
      }
   }
   GROUP "Scene Identification" {
      ATTRIBUTE "Solar Angular Dependency Models Set Version" {
         DATATYPE  H5T_STRING {
            STRSIZE 13;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "CERES_TRMM.1"
         }
      }
      ATTRIBUTE "Thermal Angular Dependency Models Set Version" {
         DATATYPE  H5T_STRING {
            STRSIZE 7;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SCALAR
         DATA {
         (0): "RMIB.3"
         }
      }
      DATASET "Aerosol Optical Depth IR 1.6" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.04
            }
         }
      }
      DATASET "Aerosol Optical Depth VIS 0.6" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.04
            }
         }
      }
      DATASET "Aerosol Optical Depth VIS 0.8" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.04
            }
         }
      }
      DATASET "Cloud Cover" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.01
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 8;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Percent"
            }
         }
      }
      DATASET "Cloud Optical Depth (logarithm)" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.00025
            }
         }
      }
      DATASET "Cloud Phase" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.01
            }
         }
         ATTRIBUTE "Unit" {
            DATATYPE  H5T_STRING {
               STRSIZE 34;
               STRPAD H5T_STR_NULLTERM;
               CSET H5T_CSET_ASCII;
               CTYPE H5T_C_S1;
            }
            DATASPACE  SCALAR
            DATA {
            (0): "Percent (Water=0%,Mixed,Ice=100%)"
            }
         }
      }
      DATASET "Dust Detection" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
         ATTRIBUTE "Quantisation Factor" {
            DATATYPE  H5T_IEEE_F64BE
            DATASPACE  SCALAR
            DATA {
            (0): 0.01
            }
         }
      }
      DATASET "Solar Angular Dependency Model" {
         DATATYPE  H5T_STD_I16BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
      }
      DATASET "Surface Type" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
      }
      DATASET "Thermal Angular Dependency Model" {
         DATATYPE  H5T_STD_U8BE
         DATASPACE  SIMPLE { ( 1237, 1237 ) / ( 1237, 1237 ) }
      }
   }
   GROUP "Times" {
      DATASET "Time (per row)" {
         DATATYPE  H5T_STRING {
            STRSIZE 22;
            STRPAD H5T_STR_NULLTERM;
            CSET H5T_CSET_ASCII;
            CTYPE H5T_C_S1;
         }
         DATASPACE  SIMPLE { ( 1237 ) / ( 1237 ) }
      }
   }
}
}
