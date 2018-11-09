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

% Householder Bidiagonalization of matrix A, where A=U*B*V' with U and V being orthogonal
% and B having elements on the main diagonal and first upper diagonal.
% 
% Based on
%  - Bidiag (v1.0.0.0) by Paul Godfrey, <pgodfrey@intersil.com>, Intersil Corp., 8-12-2002 [1]
%  - Algorithm 5.4.2 (Householder Bidiagonalization) [2, page 284ff.] and Errata [7]
%  - Algorithm 5.1.1 (Householder Vector) [2, page 236ff.] and Errata [7]
%
% References:
% [1] https://de.mathworks.com/matlabcentral/fileexchange/3568-bidiag
% [2] Gene Howard Golub and Charles Francis Van Loan (2013): Matrix Computations [4th ed.]
% [3] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%205/Functions/House.m
% [4] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%205/Functions/HouseBidiag.m
% [5] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%205/Functions/BackAccum.m
% [6] https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%205/Demos/ShowHouseBidiag.m
% [7] https://www.cs.cornell.edu/cv/GVL4/Errata.htm

% TODO: Replace with more efficient algorithms from [3], [4], [5] and [6].

function [U,b,V] = bidiag_level0(A, mode)
    coder.varsize('A', 'b');
    
    assert(mode == decompose_mode.complete); % no other mode supported yet

    b = A; % make copy of A in order to keep A constant / untouched

    [m,n]=size(b);
    k=min(m,n);
    U=eye(m);
    V=eye(n);
    
    for j=1:k
        [v,beta] = house(b(1:end, j));
        P = (eye(m)-beta*v*v');
        b = P * b;
        U = P * U;
        
        if j <= k-2
            [v,beta] = house(b(j,1:end)');
            P = (eye(n)-beta*v*v');
            b = b * P;
            V = V * P;
        end
    end
    
    U=U';
    if nargout<=1
        U=b;
    end
end

function [v, beta] = house(x)
    m = length(x);
    o = x(2:m)' * x(2:m);
    v = [ 1;
          x(2:m) ];
    if o == 0 && x(1) >= 0
        beta = 0;
    elseif o == 0 && x(1) < 0
        beta = 2;
    else
        my = sqrt(x(1)^2 + o);
        if x(1) <= 0
            v(1) = x(1) - my;
        else
            v(1) = -o/(x(1)+my);
        end
        beta = 2*v(1)^2/(o+v(1)^2);
        v = v/v(1);
    end
end
