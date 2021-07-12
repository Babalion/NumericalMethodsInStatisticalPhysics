function [O_mean,O_std,O_var,O_var_err,O_weight,O_corrLength] = ReduceObservable(O_data_dist,temp_dist)

O_data=gather(O_data_dist);
temp=gather(temp_dist);
[C,ia,~]=unique(temp);
numOfTemps=length(C);


O_mean=zeros([numOfTemps,1]);
O_std=zeros([numOfTemps,1]);
O_var=zeros([numOfTemps,1]);
O_var_err=zeros([numOfTemps,1]);
O_corrLength=zeros([numOfTemps,1]);

ia(end+1)=length(temp);
for i=1:numOfTemps
    indices=ia(i):(ia(i+1)-1);
    if(temp(indices(1))~=temp(indices(end)))
        temp(indices(1))
        temp(indices(end))
        error("indices do not match");
    end
    O_mean(i)=mean(O_data(indices));
    O_std(i)=std(O_data(indices));
    
    numBootstrp=min(5E2,length(indices));
    if length(indices)>1E4
        %[ci, ~]=bootci(numBootstrp,{@var,O_data(indices)},'Options',statset('UseParallel',true));
        O_var(i)=var(O_data(indices));
        O_var_err(i)=0;
    else
        [ci, ~]=bootci(numBootstrp,{@var,O_data(indices)},'Options',statset('UseParallel',true));
        O_var(i)=mean(ci);
        O_var_err(i)=diff(ci)/2;
    end
    
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

