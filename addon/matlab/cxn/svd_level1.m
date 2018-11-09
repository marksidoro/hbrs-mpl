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

% Singular value decomposition of m*n matrix A, where A=U*S*V' with U and V being orthogonal
% and S having singular values on the main diagonal.
%
% Based on 
%  - Algorithm 8.6.1 (Golub-Kahan SVD Step) [1, page 491] and Errata [5]
%  - Algorithm 8.6.2 (The SVD Algorithm) [1, page 492] and Errata [5]
%
% References:
% [1] Gene Howard Golub and Charles Francis Van Loan (2013): Matrix Computations [4th ed.]
% [2] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%208/Functions/GKsvdStep.m
% [3] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%208/Demos/ShowGKsvd.m
% [4] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%205/Functions/Givens.m
% [5] https://www.cs.cornell.edu/cv/GVL4/Errata.htm

function [U,S,V] = svd_level1(A, mode)
    coder.varsize('A', 'x');
    
    x = A; % make copy of A in order to keep A constant / untouched
    
    % TODO: Replace dummy implementation with SVD algorithm from [2], [3] and [4].
    
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
