int compareStrings(char *array1, char *array2)
{
    int i;
    int response = 1;
    i = 0;

    while(array1[i] == array2[i] && response == 1 )
    {
        if(array1[i] == '\0' || array2[i] == '\0'){
            response = 0;
        }
        i++;
    }

    return response;
}


int copyStrings(char s1[10], char s2[10])
{
		int i;

    for(i = 0; s1[i] != '\0'; ++i)
    {
        s2[i] = s1[i];
    }

    s2[i] = '\0';

    return 0;
}
