; ModuleID = 'output.ir'
source_filename = "test"

define i32 @main() {
EntryBlock:
  %addresult = add i32 2, 3
  %subresult = sub i32 2, 3
  ret i32 %addresult
}
