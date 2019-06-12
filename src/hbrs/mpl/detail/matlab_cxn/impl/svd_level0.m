% Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
% 
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with this program. If not, see <http://www.gnu.org/licenses/>.

% NOTE: This requires 'MATLAB_*_Coder_xzsvdc_m.patch' applied to MATLAB!
% MATLAB CODER error message:
%  ??? Variable 'Vf' is not fully defined on some execution paths.
%  
%  Error in ==> xzsvdc Line: 421 Column: 89

function [U,S,V] = svd_level0(A, mode) %#codegen
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    
    switch(mode)
        case decompose_mode.complete
            [U,S,V] = svd(x);
        case decompose_mode.economy
            [U,S,V] = svd(x, 'econ');
        case decompose_mode.zero
            [U,S,V] = svd(x, 0);
        otherwise
            assert(false);
            U = [];
            S = [];
            V = [];
    end
end
