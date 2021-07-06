function [temp, en, mag] = importfileAppend(path,temp,en,mag)
numOfTemps=readmatrix(path,"FileType","text","Delimiter","\t","Range",'B1:B1');
numOfIterations=readmatrix(path,"FileType","text","Delimiter","\t","Range",'B2:B2');
it=numOfTemps*numOfIterations;

[~, temp(end+1:end+it), mag(end+1:end+it), en(end+1:end+it), ~, ~] = importfile(path,1,it);
[temp,I]=sort(temp);
mag=mag(I);
en=en(I);


end