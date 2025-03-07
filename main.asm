main:
  ldm 9
  call test
  hlt
test:
  ldm 11
  call test2
  ret
test2:
  ldm 15
  call test3
  ret
test3:
  ldm 1
  call test4
  ret
test4:
  ldm 2
  call test5
  ret
test5:
  ldm 3
  call test6
  ret
test6:
  ldm 4
  call test7
  ret
test7:
  ldm 5
  ret