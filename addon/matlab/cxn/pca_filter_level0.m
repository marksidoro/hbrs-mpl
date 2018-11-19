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

function [data,latent] = pca_filter_level0(A, filtered)
    coder.varsize('A', 'filtered');

    [m,n] = size(A);
    if m-1<n
        assert(size(filtered, 1) == m-1);
    else
        assert(size(filtered, 1) == min([m,n]));
    end
	
    [coeff,score,latent,~,~,mu] = pca(A, 'Economy', true);
		
%   [m,n] = size(coeff);
%   for i = 1:n
%       if filter(i) == false
%            coeff(:,i) = 0;
%        end
%    end
%   data = (score * transpose(coeff)) + mu;
    
    data = score * diag(filtered) * coeff';
    [m,n] = size(data);
    for i = 1:m
        for j = 1:n
            data(i,j) = data(i,j) + mu(j);
        end
    end
    
%   data = score * diag(filtered) * coeff' + mu;
%   data = score * coeff' + mu;
    
    assert(isequal(size(data), size(A)));
end
