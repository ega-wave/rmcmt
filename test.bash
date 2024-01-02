
function assert_equal_slaster() {
  set -f
  echo $1 | ./rmcmt_slaster 2>/dev/null >/dev/null ; status=$?
  set +f

  if [[ $status != $2 ]]; then
    exit 1;
  fi
}

assert_equal_slaster '' 0
assert_equal_slaster '/' 0
assert_equal_slaster '/*' 1
assert_equal_slaster '/* ' 1
assert_equal_slaster '/* /' 1
assert_equal_slaster '/* *' 1
assert_equal_slaster '/* */' 0

