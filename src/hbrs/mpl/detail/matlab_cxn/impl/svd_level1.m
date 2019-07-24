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

function [U,B,V] = svd_level1(A, mode)
    %Algorithm 8.6.2 (The SVD Algorithm) on page 492
    %   Given matrix A (which is real and m-by-n) (m>=n) the following
    %   algorithm computes the SVD, where U (which is real and m-by-m) is
    %   orthogonal and V (which is real and n-by-n) is orthogonal.
    %
    %   Contrary to the book this algorithm also computes U' and V. Taking
    %   matrices U' and V from the bidiagonalization it overwrites them by
    %   applying the Givens transformations calculated in this algorithm to
    %   them.
    %
    %   Instead of overwriting A this algorithm stores A, U' and V and
    %   returns them as A U and V.

    if mode ~= decompose_mode.complete
        [U,B,V] = svd_level0(A, mode);
        return
    end
    
    assert(mode == decompose_mode.complete); % no other mode supported yet

    % TODO: Implement decompose_mode.economy and decompose_mode.zero
    % TODO: Extend algorithm for m < n
    % TODO: Sort singular values in S as nonnegative real numbers listed in decreasing order

    [U,B,V] = bidiag_level0(A, mode);
    [m,n] = size(A);
    assert(m >= n);

    q = 0;
    p = 0;

    while q ~= n
        for i=1:n-1
            if abs(B(i,i+1)) <= eps
                B(i,i+1) = 0;
            end
        end

        % Find the largest q and the smallest p such that if
        %     -----------------
        %     | B11   0    0  |    p
        %     |               |
        % B = |  0   B22   0  |  n-p-q
        %     |               |
        %     |  0    0   B33 |    q
        %     -----------------
        %        p  n-p-q  q
        % then B33 is diagonal and B22 has a nonzero superdiagonal.

        % First find q
        for q=0:n-1
            if q == n-1
                q = n;
                break;
            elseif B(n - q - 1, n - q) ~= 0
                break;
            end
        end
        if q < n
            % Second find p
            % In the book this part is before the if condition. We
            % moved it inside the if condition for optimization.
            for p=n-q-1:-1:1
                if B(p, p+1) == 0
                    break;
                elseif p == 1
                    p = 0;
                    break;
                end
            end

            % if any diagonal entry in B22 is zero, then zero the
            % superdiagonal entry in the same row.

            zeroFound = false; % Turns true once a zero is found in the bidiagonal of B22
            for i=1+p:n-q
                if B(i, i) == 0
                    zeroFound = true;
                    if i < n-q
                        for j=i+1:n-q
                            % The function arguments in the next line are copied from the Matlab implementation of the Book(Link: https://www.cs.cornell.edu/cv/GVL4/M-Files/Chapter%208/Demos/ShowGKsvd.m).
                            theta = givens(-B(j,j), B(i,j));
                            B = G(i,j,n,theta)' * B;
                            U = U * G(i,j,n,theta);
                        end
                    else
                        for j=n-q-1:-1:p+1
                            % Same as above
                            theta = givens(B(j,j), -B(j,n-q));
                            B = B * G(j,n-q,n,theta);
                            V = V * G(j,n-q,n,theta);
                        end
                    end
                end
            end

            if zeroFound
            else
              [U, B(p+1:n-q, p+1:n-q), V] = svdStep(U, B(p+1:n-q, p+1:n-q), V, p); % Apply Algorithm 8.6.1 to B22
                % In the book there is another line here that looks
                % like this:
                % B = diag(Ip,U,Iq+m-n)' B diag(Ip,V,Iq)
                % But that line is actually not part of the algorithm.
                % 
            end
        end
    end % while q ~= n
end % function svd

function [U,B,V] = svdStep(U,B,V,p)
    %Algorithm 8.6.1 (Golub-Kahan SVD Step) on page 491
    %   Given a bidiagonal matrix B (which is of real numbers and of the
    %   dimensions m-by-n) having no zeros on its diagonal or superdiagonal,
    %   the following algorithm overwrites B with the bidiagonal matrix
    %   B=U'BV where U and V are orthogonal and V is essentially the
    %   orthogonal matrix that would be obtained by applying Algorithm
    %   8.3.2 to T=B'B.
    %
    %   Contrary to the book this algorithm also computes U' and V. Given
    %   matrices U' and V it overwrites them by applying the Givens
    %   transformations calculated in this algorithm to them.
    %
    %   p is necessary for the calculation of U
    %   and V.

    [m,n] = size(B);
    [~,vn] = size(V);
    [um,~] = size(U);

    % Let mu be the eigenvalue of the trailing 2-by-2 submatrix of
    % T=B'B that is closer to tnn.
    %
    % Calculate mu
    T = B' * B;
    E = eig(T(n-1:n,n-1:n));
    e1 = E(1);
    e2 = E(2);
    if abs(e1 - T(n,n)) < abs(e2 - T(n,n))
        mu = e1;
    else
        mu = e2;
    end
    % end Calculate mu

    y = T(1,1) - mu;
    z = T(1,2);

    for k=1:n-1
        % Determine c = cos(theta) and s = sin(theta) such that
        theta = givens(y,z);
        B = B * G(k,k+1,n,theta);
        V = V * G(p+k,p+k+1,vn,theta);

        y = B(k,k);
        z = B(k+1,k);

        % Determine c = cos(theta) and s = sin(theta) such that
        theta = givens(y,z);
        B = G(k,k+1,m,theta)' * B;
        U = U * G(p+k,p+k+1,um,theta);
        if k < n - 1
            y = B(k,k+1);
            z = B(k,k+2);
        end
    end % for k=1:n-1
end % function svdStep

function [R] = G(i,k,n,theta)
    %Creates a n-by-n Givens Rotation Matrix
    %   The values c and s are retrieved from G

    R = eye(n);
    R(i,i) = theta(1,1);
    R(i,k) = theta(1,2);
    R(k,i) = theta(2,1);
    R(k,k) = theta(2,2);

end % function G

function [theta] = givens(a,b)
    %Algorithm 5.1.3 (Calculate Givens scalars) on page 240
    %   --     --T   --   --         --   --
    %   |       |    |     |         |     |
    %   |  c s  |    |  a  |         |  r  |
    %   |       |    |     |    =    |     |
    %   | -s c  |    |  b  |         |  0  |
    %   |       |    |     |         |     |
    %   --     --    --   --         --   --
    %   
    %   Return a matrix like this:
    %   --     --
    %   |       |
    %   |  c s  |
    %   |       |
    %   | -s c  |
    %   |       |
    %   --     --

    theta = planerot([a;b])';
end % function givens
