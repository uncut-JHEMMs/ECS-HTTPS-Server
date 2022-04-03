BEGIN{count = 0}

{
    totalSwapMem = $2
    swapUsed[count] = $3
    swapFree[count] = $4
    count += 1
    
}

END{
    for(i = 0; i < count; i++)
	print totalSwapMem, swapUsed[i], swapFree[i] > "tmpfile" 
}
