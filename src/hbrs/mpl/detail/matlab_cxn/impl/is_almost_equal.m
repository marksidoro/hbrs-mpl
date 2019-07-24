% Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

% References:
%  https://de.mathworks.com/matlabcentral/fileexchange/15816-isalmost
%  https://de.mathworks.com/matlabcentral/fileexchange/11305-springlab

function [tf] = is_almost_equal(A, B, tol)
    if nargin==2
        tol=sqrt(eps(max(abs(A),abs(B))));
    end
    
    tf=abs((A-B))<=abs(tol);
end
