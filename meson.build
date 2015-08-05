# Build for power monitor
# Brandon Edens

project('power-monitor', 'c')
add_global_arguments('-std=gnu11', language : 'c')
add_global_arguments('-fdiagnostics-color=always', language : 'c')
srcs = ['main.c', 'multicast_server.c']

exe = executable('power-monitor', sources : srcs)
exe = executable('power-monitor-client', sources : 'client.c')


# vim:syntax=python:
