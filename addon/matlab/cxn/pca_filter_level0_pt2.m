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

function [data] = pca_filter_level0_pt2(coeff, score, mu, filtered)
    coder.varsize('coeff', 'score', 'mu', 'filtered');
    
    data = score * diag(filtered) * coeff';
    [m,n] = size(score);
    for i = 1:m
        for j = 1:n
            data(i,j) = data(i,j) + mu(j);
        end
    end
end
