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

% Principal component analysis (PCA)
%
% Uses Singular value decomposition (SVD) algorithm.
% Ready for code generation using MATLAB Coder.
%
% References:
%  ${Matlab_ROOT_DIR}/toolbox/stats/stats/pca.m

% NOTE: This requires 'MATLAB_*_Coder_xzsvdc_m.patch' applied to MATLAB!
% MATLAB CODER error message:
%  ??? Variable 'Vf' is not fully defined on some execution paths.
%  
%  Error in ==> xzsvdc Line: 421 Column: 89

function [coeff,score,latent,mu] = pca_level0(A, Economy)
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    [m,n] = size(x)
    [coeff,score,latent,~,~,mu] = pca(x, 'Economy', Economy);
    
    if (~Economy && (m < n)) % equals decompose_mode::complete
        latent = latent(1:m) % Why is length of latent(1:n) in matlab?
    end
end
