## tap_testname = group creation - nested groups
HDF5 "test04.h5" {
GROUP "/" {
    GROUP "abc" {
        GROUP "def" {
        }
    }
    GROUP "ghi" {
        GROUP "jkl" {
            GROUP "mno" {
                GROUP "pqr" {
                }
                GROUP "stu" {
                }
            }
        }
        GROUP "vwx" {
        }
    }
}
}
