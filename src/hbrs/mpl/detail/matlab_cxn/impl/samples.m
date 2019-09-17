% Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

function [DS] = samples()
   hald_mat = coder.load('hald.mat');
   DS = { ...
       hald_mat.ingredients, ...
       hald_mat.ingredients', ...
       reshape( 1:9, 3, 3).', ...
       [ 1 2 3 4
         5 6 7 8 ], ...
       [ 1  0  1
        -1 -2  0
         0  1 -1 ], ...
       [ 1 2
         3 4
         5 6
         7 8 ], ...
       [ 1 2 3 4 5
         6 7 8 9 0 ], ...
        [ 1     0     1
         -1    -2     0
          0     1    -1 ], ...
        [ 1     2
          3     4
          5     6
          7     8 ], ...
        [ 12.0  -51.0   4.0
          06.0  167.0 -68.0
          -4.0   24.0 -41.0 ] ...
   };
end
