#!/usr/bin/bash
# Provides: server
# Required-View-Cmd: $SERVER_NAME $cmd $key
### END INIT INFO


SERVER_NAME=server


get_pid() {
  PID_MASTER_PROCESS=$(head -n 1 pid_file.txt)
}


get_has_server_started() {
  HAS_SERVER_STARTED=$(ps aux | grep ./"$SERVER_NAME".out | wc -l)
}


start() {
  if [ ! -d cmake-build-debug ]; then
    echo "To start the server you need to execute 'sudo ./$SERVER_NAME.sh build'"
    return
  fi

  get_has_server_started
  if [ "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server has already been started!"
    return
  fi

  if [ -f error.log ]; then
    rm error.log
  fi
  if [ -f access.log ]; then
    rm access.log
  fi

  touch access.log
  touch error.log

  if [ -f "$SERVER_NAME".out ]; then
    rm "$SERVER_NAME".out
  fi

  cp ./cmake-build-debug/"$SERVER_NAME".out "$SERVER_NAME".out
  ./"$SERVER_NAME".out

  echo "Server has been started!"
}


test() {
  if [ ! -d cmake-build-debug ]; then
    echo "To start the server you need to execute 'sudo ./$SERVER_NAME.sh build'"
    return
  fi

  if [ -f test.out ]; then
    rm test.out
  fi

  cp ./cmake-build-debug/test/test.out test.out
  ./test.out
}


stop_soft() {
  get_has_server_started
  if [ ! "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server has not been started yet!"
    return
  fi

  get_pid
  kill -1 "$PID_MASTER_PROCESS"

  echo "Server has been stopped!"
}


stop_hard() {
  get_has_server_started
  if [ ! "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server has not been started yet!"
    return
  fi

  get_pid
  kill -2 "$PID_MASTER_PROCESS"

  echo "Server has been stopped!"
}


reload_soft() {
  get_has_server_started
  if [ ! "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server has not been started yet!"
    return
  fi

  get_pid
  kill -13 "$PID_MASTER_PROCESS"

  echo "Server has been reloaded!"
}


reload_hard() {
  get_has_server_started
  if [ ! "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server has not been started yet!"
    return
  fi

  get_pid
  kill -14 "$PID_MASTER_PROCESS"

  echo "Server has been reloaded!"
}


status() {
  get_has_server_started
  if [ "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Server is running!"
  else
    echo "Server is down!"
  fi
}


create_config() {
  get_has_server_started

  get_has_server_started
  if [ "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Can't create config file while server is running!"
    return
  fi

  if [ -f settings/"$SERVER_NAME".conf ]; then
    echo "The config file already exists";
  else
    cp ."$SERVER_NAME".conf settings/"$SERVER_NAME".conf
  fi
}


build() {
  get_has_server_started
  if [ "$HAS_SERVER_STARTED" \> 1 ]; then
    echo "Can't build project while server is running!"
    return
  fi

  if [ -d cmake-build-debug ]; then
    cd cmake-build-debug
    make
    cd ..
  else
    mkdir cmake-build-debug
    cd cmake-build-debug
    cmake ..
    make
    cd ..
  fi
}


case $1 in
  start)
    start
  ;;

  test)
    test
  ;;

  stop)
    shift
    case $1 in
      --soft)
        stop_soft
      ;;

      --hard)
        stop_hard
      ;;

      *)
        echo "Usage: stop <option>";
        echo

        echo -e "\t--hard\t\tdon't get new connections and don't handle remaining ones";
        echo -e "\t--stop\t\tdon't get new connections and handle remaining ones\n";
      ;;
    esac
  ;;

  reload)
    shift
    case $1 in
      --soft)
        reload_soft
      ;;

      --hard)
        reload_hard
      ;;

      *)
        echo "Usage: reload <option>";
        echo

        echo -e "\t--hard\t\tdon't get new connections and don't handle remaining ones";
        echo -e "\t--stop\t\tdon't get new connections and handle remaining ones\n";
      ;;
    esac
  ;;

  status)
    status
  ;;

  build)
    build
  ;;

  create)
    shift
    case $1 in
      --config)
        create_config
      ;;

      *)
        echo "Usage: reload <option>";
        echo

        echo -e "\t--config\t\tcreate template config file";
      ;;
    esac
  ;;

  *)
    get_has_server_started
    if [ "$HAS_SERVER_STARTED" \> 1 ]; then
      echo "Usage: sudo ./server.sh <command> [<option>]";
      echo

      echo -e "\tstop\t\tstop the server";
      echo -e "\treload\t\treload the server";
      echo -e "\tstatus\t\tget status about server's activity";
    else
      echo "Usage: sudo ./server.sh <command> [<option>]";
      echo

      echo -e "\tstart\t\tstart the server";
      echo -e "\tbuild\t\tbuild the project";
      echo -e "\tstatus\t\tget status about server's activity";
      echo -e "\tcreate\t\tcreate file";
    fi
  ;;
esac

echo
exit 0
