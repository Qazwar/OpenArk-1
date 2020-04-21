




.code



GetCurrentPrcb proc

mov rax,gs:[20h]
ret

GetCurrentPrcb endp

FakeTerminate proc

mov r11,rsp
mov     qword ptr [r11+18h],rbx
mov     dword ptr [rsp+10h],edx
mov rax, 0fffff8000418f58bh
jmp  rax
FakeTerminate endp

end 
