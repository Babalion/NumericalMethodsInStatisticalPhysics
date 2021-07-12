function [obsRw,obsRw_err] = reweightObs(obs,energies,tempOfEnergies)
tempIndex=10;
newT=temp_red(tempIndex)+0.2;

tempReweight=@(energies)(sum(obs(tempIndex)*exp(-(1/newT-1/temp_red(tempIndex))*energies))/sum(exp(-(1/newT-1/temp_red(tempIndex))*energies)));
[c, bs]=bootci(100,{tempReweight,en4((tempIndex-1)*numOfIterations+1:tempIndex*numOfIterations) },'Options',statset('UseParallel',true))
end

