function [O_mean,O_std,O_var,O_weight,O_corrLength] = ReduceObservable(O_data,temp)

[C,ia,~]=unique(temp);
numOfTemps=length(C);


O_mean=zeros([numOfTemps,1]);
O_std=zeros([numOfTemps,1]);
O_var=zeros([numOfTemps,1]);
O_corrLength=zeros([numOfTemps,1]);

ia(end+1)=length(temp);
for i=1:numOfTemps
    O_mean(i)=mean(O_data(ia(i):ia(i+1)));
    O_std(i)=std(O_data(ia(i):ia(i+1)));
    O_var(i)=var(O_data(ia(i):ia(i+1)));
    
    maxlag=round((ia(i+1)-ia(i))/5,0);
    [acf,~,bounds]=autocorr(O_data(ia(i):ia(i+1)),maxlag);
    findIndex=find( acf <bounds(1), 1 );
    
    if(isempty(findIndex))
        O_corrLength(i)=maxlag;   
    else
        O_corrLength(i)=findIndex;
    end
end
O_weight=1./O_std.^2;
end

