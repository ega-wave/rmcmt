
function assert_equal() {
  set -f
  echo $1 | ./rmcmt_slaster 2>/dev/null >/dev/null ; status=$?
  set +f

  if [[ $status != $2 ]]; then
    exit 1;
  fi
}

assert_equal '' 0
assert_equal '/' 0
assert_equal '/*' 1
assert_equal '/* ' 1
assert_equal '/* /' 1
assert_equal '/* *' 1
assert_equal '/* */' 0

