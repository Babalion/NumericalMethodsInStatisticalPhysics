function E = fitEnergy(T,Tcrit,E0,gamma)
E=E0*(T-Tcrit).^(-gamma);
E(imag(E) ~= 0)=0;
E(isinf(E))=0;
end

