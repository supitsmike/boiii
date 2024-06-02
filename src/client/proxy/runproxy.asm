.DATA

EXTERN ProcAddress : qword

.CODE

run_proxy PROC
	jmp [ProcAddress]
run_proxy ENDP

END
