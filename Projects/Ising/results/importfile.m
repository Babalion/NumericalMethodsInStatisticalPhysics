function [N, temp, magnetization, energy, susceptibility, heatCapacity] = importfile(filename,dataLineStart,dataLineEnd)
%IMPORTFILE Import data from a text file
%  [N, TEMP, MAGNETIZATION, ENERGY, SUSCEPTIBILITY, HEATCAPACITY] =
%  IMPORTFILE(FILENAME) reads data from text file FILENAME for the
%  default selection.  Returns the data as column vectors.

dataLines = [dataLineStart+4, dataLineEnd+4];

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 6);

% Specify range and delimiter
opts.DataLines = dataLines;
opts.Delimiter = "\t";

% Specify column names and types
opts.VariableNames = ["N", "temp", "magnetization", "energy", "susceptibility", "heatCapacity"];
opts.VariableTypes = ["double", "double", "double", "double", "string", "string"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["susceptibility", "heatCapacity"], "WhitespaceRule", "preserve");
opts = setvaropts(opts, ["susceptibility", "heatCapacity"], "EmptyFieldRule", "auto");

% Import the data
tbl = readtable(filename, opts);

%% Convert to output type
N = tbl.N;
if(max(N)~=min(N))
    error("wrong separation of data! (correct dataLineStart and dataLineEnd)")
end
N=N(1);
temp = tbl.temp;
magnetization = tbl.magnetization;
energy = tbl.energy;
susceptibility = tbl.susceptibility;
heatCapacity = tbl.heatCapacity;
end