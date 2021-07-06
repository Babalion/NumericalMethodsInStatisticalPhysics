function E = fitMagnetization(T,Tcrit,M0,beta)
E=M0*(T-Tcrit).^(-beta);
E(imag(E) ~= 0)=1;
E(isinf(E))=0;
end