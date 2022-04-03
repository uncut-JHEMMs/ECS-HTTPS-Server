BEGIN{count = 0}
{
    if($4 != "" && $10 != ""){
	diskRead[count] = $4
	diskWrite[count] = $10
	count += 1
    }
}
END{
    for(i = 0; i < count; i++){
	print diskRead[i], diskWrite[i] > "tmpfile"
    }
}
