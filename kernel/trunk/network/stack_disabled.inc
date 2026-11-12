;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                 ;;
;;  STACK_DISABLED.INC                                             ;;
;;                                                                 ;;
;;  Network-free replacement for network/stack.inc.                ;;
;;  Selected in kernel32.inc when NETWORK_DISABLED is defined.     ;;
;;  Keeps every symbol the rest of the kernel (main loop, syscall  ;;
;;  table, exports table) refers to, but implements none of the    ;;
;;  TCP/IP stack: no sockets, no devices, no packet processing.    ;;
;;                                                                 ;;
;;  Distributed under terms of the GNU General Public License      ;;
;;                                                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Error codes that non-network kernel code (posix/, fs/) also relies on.
ENOBUFS                 = 1
EINPROGRESS             = 2
EOPNOTSUPP              = 4
EWOULDBLOCK             = 6
ENOTCONN                = 9
EALREADY                = 10
EINVAL                  = 11
EMSGSIZE                = 12
ENOMEM                  = 18
EADDRINUSE              = 20
EADDRNOTAVAIL           = 21
ECONNRESET              = 52
ECONNABORTED            = 53
EISCONN                 = 56
ETIMEDOUT               = 60
ECONNREFUSED            = 61

; Called once from kernel.asm during boot.
stack_init:
        ret

; Called from the main OS loop; nothing to do.
proc stack_handler
        ret
endp

; ZF=1 -> no pending work for the main OS loop.
proc stack_handler_has_work?
        xor     eax, eax
        ret
endp

; Called when a process terminates; it can own no sockets.
socket_process_end:
        ret

; System functions 74, 75 and 76: report "not available".
align 4
sys_network:
sys_socket:
sys_protocols:
        or      dword [esp + SYSCALL_STACK.eax], -1
        ret

; Exported to drivers (NetRegDev, NetUnRegDev, ...). A driver that tries
; to register a network device is told the request failed.
align 4
net_add_device:
net_remove_device:
net_ptr_to_num:
net_link_changed:
eth_input:
        or      eax, -1
        ret

; NetAlloc / NetFree: no buffers are ever handed out.
align 4
net_buff_alloc:
        xor     eax, eax
        ret

align 4
net_buff_free:
        ret
