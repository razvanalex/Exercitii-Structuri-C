function[s]=dim(n)
  fid=fopen(n)
  fseek(fid, 0, 'eof');
  s=ftell(fid);
  fclose(fid);
endfunction