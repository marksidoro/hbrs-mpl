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

function [coeff,score,latent,mu] = pca_level0(A, Economy, Center, Normalize)
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    [~,n] = size(x);
    
    if Normalize
        % Let x_i be a column of x.
        % If and only if all entries in x_i are zero, then variance(x_i)=0.
        % If and only if variance(x_i)=0, then 1/variance(x_i)=inf.
        % Normalization includes a scalar-multiplication sqrt(1/var(x_i)) * x_i
        % which will results in a column of NaN's iff x_i has only zeros.
        % Input to MATLAB Coder's SVD must not contain NaN's and thus we just
        % replace all inf's in vw with 1's and thus "skipping" normalization.
        vVariableWeights = 1./nanvar(x,0,1);
        vVariableWeights(isinf(vVariableWeights)) = 1;
        
        [coeff,score,latent,~,~,mu] = pca(x, 'Economy', Economy, ...
            'Centered', Center, 'VariableWeights', vVariableWeights);
    else
        [coeff,score,latent,~,~,mu] = pca(x, 'Economy', Economy, ...
            'Centered', Center, 'VariableWeights', ones(1,n,'like',x));
    end
end
