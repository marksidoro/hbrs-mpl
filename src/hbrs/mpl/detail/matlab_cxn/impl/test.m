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

% TODO: replace isequal with is_almost_equal to better handle floating point arithmetics

function test()
    eig_test();
    dmd_test();
    pca_filter_test();
    pca_test();
    svd_test();
    bidiag_test();
end

function eig_test()
    % TODO: Implement!
end

function dmd_test()
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @dmd_level1, @dmd_level1};
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        [m,n] = size(dataset);
        max_rank = min(m,n);
        
        runs_per_fun = max_rank;
        eigenvalues  = cell(1,funs_sz*runs_per_fun);
        modes        = cell(1,funs_sz*runs_per_fun);
        coefficients = cell(1,funs_sz*runs_per_fun);

        for i = 1:funs_sz
            fun = funs{1,i};
            for target_rank = 1:max_rank
                x1 = dataset(:,1:n-1);
                x2 = dataset(:,2:n);
                
                [ ...
                    eigenvalues{1,i+funs_sz*(target_rank-1)}, ...
                    modes{1,i+funs_sz*(target_rank-1)}, ...
                    coefficients{1,i+funs_sz*(target_rank-1)} ...
                ] = fun(x1, x2, target_rank);
            end
        end
        
        for i = 1:funs_sz-1
            % TODO: Properly test eigenvalues, modes and coefficients
            for target_rank = 1:max_rank
                assert(isequal( ...
                    eigenvalues{1,i+funs_sz*(target_rank-1)}, ...
                    eigenvalues{1,i+funs_sz*(target_rank-1)+1} ...
                ));
                assert(isequal( ...
                    modes{1,i+funs_sz*(target_rank-1)}, ...
                    modes{1,i+funs_sz*(target_rank-1)+1} ...
                ));
                assert(isequal( ...
                    coefficients{1,i+funs_sz*(target_rank-1)}, ...
                    coefficients{1,i+funs_sz*(target_rank-1)+1} ...
                ));
            end
        end
    end
end

function pca_filter_test()
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @pca_filter_level0, @pca_filter_level0};
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        [m,n] = size(dataset);
        
        if m-1<n % and Economy:=true
            p = m-1;
        else
            p = min(m,n);
        end
        
        runs_per_fun = 2+p;
        data = cell(1,funs_sz*runs_per_fun);
        latent = cell(1,funs_sz*runs_per_fun);

        for i = 1:funs_sz
            fun = funs{1,i};
            % TODO: test other cases, e.g. Economy:=false,
            %       Center:=false or Normalize := false
            
            [data{1,i}, latent{1,i}] = ...
                fun(dataset, diag(ones(p)), ...
                    true, ... % Economy
                    true, ... % Center
                    true ... % Normalize
                );
            
            [data{1,i+funs_sz}, latent{1,i+funs_sz}] = ...
                fun(dataset, diag(zeros(p)), ...
                    true, ... % Economy
                    true, ... % Center
                    true ... % Normalize
                );
            
            for j = 1:p
                filtered = diag(zeros(p));
                filtered(j) = 1;
                [data{1,i+funs_sz*(1+j)}, latent{1,i+funs_sz*(1+j)}] = ...
                    fun(dataset, filtered, ...
                        true, ... % Economy
                        true, ... % Center
                        true ... % Normalize
                    );
            end
        end

        for i = 1:funs_sz-1
            [m,n] = size(dataset);
            for mi = 1:m
                for ni = 1:n
                    a = dataset(mi,ni);
                    b = data{1,i}(mi,ni);
                    tol = eps(10^12);
                    assert(abs(b-a) < tol);
                end
            end
            %assert(isequal(dataset, data{1,i}));
            
            assert(isequal(data{1,i}, data{1,i+1}));
            assert(isequal(latent{1,i}, latent{1,i+1}));

            assert(isequal(data{1,i+funs_sz}, data{1,i+funs_sz+1}));
            assert(isequal(latent{1,i+funs_sz}, latent{1,i+funs_sz+1}));
            
            for j = 1:p
                assert(isequal(data{1,i+funs_sz*(1+j)}, data{1,i+funs_sz*(1+j)+1}));
                assert(isequal(latent{1,i+funs_sz*(1+j)}, latent{1,i+funs_sz*(1+j)+1}));
            end
        end
    end
end

function pca_test()
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @pca_level0, @pca_level1, @pca_level2 };
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        
        coeffs = cell(1,funs_sz*2);
        scores = cell(1,funs_sz*2);
        latents = cell(1,funs_sz*2);

        for i = 1:funs_sz
            fun = funs{1,i};
            % TODO: test other cases, e.g. Center:=false or Normalize := false
            [coeffs{1,i},scores{1,i},latents{1,i}] = ...
                fun(dataset, ...
                    true, ... % Economy
                    true, ... % Center
                    true ... % Normalize
                );
            [coeffs{1,i+funs_sz},scores{1,i+funs_sz},latents{1,i+funs_sz}] = ...
                fun(dataset, ...
                    false, ... % Economy
                    true, ... % Center
                    true... % Normalize
                );
        end

        for i = 1:funs_sz-1
            assert(isequal(coeffs{1,i}, coeffs{1,i+1}));
            assert(isequal(scores{1,i}, scores{1,i+1}));
            assert(isequal(latents{1,i}, latents{1,i+1}));

            assert(isequal(coeffs{1,i+funs_sz}, coeffs{1,i+funs_sz+1}));
            assert(isequal(scores{1,i+funs_sz}, scores{1,i+funs_sz+1}));
            assert(isequal(latents{1,i+funs_sz}, latents{1,i+funs_sz+1}));
        end
    end
end

function svd_test()
    tol=eps*10000; % TODO: Meaningful value!
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @svd_level0, @svd_level1 };
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        [m,n] = size(dataset);
        
        Us = cell(1,funs_sz*3);
        Ss = cell(1,funs_sz*3);
        Vs = cell(1,funs_sz*3);

        for i = 1:funs_sz
            fun = funs{1,i};
            [Us{1,i},Ss{1,i},Vs{1,i}] ...
                = fun(dataset, decompose_mode.complete);
            [Us{1,i+funs_sz},Ss{1,i+funs_sz},Vs{1,i+funs_sz}] ...
                = fun(dataset, decompose_mode.economy);
            [Us{1,i+funs_sz*2},Ss{1,i+funs_sz*2},Vs{1,i+funs_sz*2}] ...
                = fun(dataset, decompose_mode.zero);
        end
        
        for i = 1:funs_sz
            for j = 1:3
                U = Us{1,i+(j-1)*funs_sz};
                S = Ss{1,i+(j-1)*funs_sz};
                V = Vs{1,i+(j-1)*funs_sz};
                re = U*S*V';
                
                E=dataset-re;
                max_norm_E = max(E,[],'all');
                disp(['||A - U*S*V^T||_max=', num2str(max_norm_E)]);
                
                fro_norm_E = norm(E, 'fro');
                disp(['||A - U*S*V^T||_F=', num2str(fro_norm_E)]);
                
                scaled_residual = norm(E,'fro')/max(m,n)*eps*norm(dataset);
                disp(['||A - U*S*V^T||_F / max(m,n) * eps * ||A||_2 =', num2str(scaled_residual)]);
                
                assert(all(is_almost_equal(dataset, re, tol), 'all'));
                
                % test if anything in S except diagonal is zero
                S_zerod = zeros(size(S),'like',S);
                S_zerod(1:min(m,n),1:min(m,n)) = S(1:min(m,n),1:min(m,n)) - diag(diag(S));
                max_norm_S_zerod = max(S_zerod,[],'all');
                disp(['||S-diag(diag(S))||_max=', num2str(max_norm_S_zerod)]);
                assert(all(is_almost_equal(S_zerod, zeros(size(S)), tol), 'all'));
            end
        end

        for i = 1:funs_sz-1
            assert(all(is_almost_equal(Us{1,i}, Us{1,i+1}, tol), 'all'));
            assert(all(is_almost_equal(Ss{1,i}, Ss{1,i+1}, tol), 'all'));
            assert(all(is_almost_equal(Vs{1,i}, Vs{1,i+1}, tol), 'all'));

            assert(all(is_almost_equal(Us{1,i+funs_sz}, Us{1,i+funs_sz+1}, tol), 'all'));
            assert(all(is_almost_equal(Ss{1,i+funs_sz}, Ss{1,i+funs_sz+1}, tol), 'all'));
            assert(all(is_almost_equal(Vs{1,i+funs_sz}, Vs{1,i+funs_sz+1}, tol), 'all'));
            
            assert(all(is_almost_equal(Us{1,i+funs_sz*2}, Us{1,i+funs_sz*2+1}, tol), 'all'));
            assert(all(is_almost_equal(Ss{1,i+funs_sz*2}, Ss{1,i+funs_sz*2+1}, tol), 'all'));
            assert(all(is_almost_equal(Vs{1,i+funs_sz*2}, Vs{1,i+funs_sz*2+1}, tol), 'all'));
        end
    end
end

function bidiag_test()
    tol=eps*10000; % TODO: Meaningful value!
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @bidiag_level0, };
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        [m,n] = size(dataset);
        
        % TODO: Remove once m<n is implemented!
        if m<n
            continue
        end
        
        Us = cell(1,funs_sz*3);
        Bs = cell(1,funs_sz*3);
        Vs = cell(1,funs_sz*3);

        for i = 1:funs_sz
            fun = funs{1,i};
            [Us{1,i},Bs{1,i},Vs{1,i}] ...
                = fun(dataset, decompose_mode.complete);
            
            % TODO: Uncomment once other modes are implemented!
            %[Us{1,i+funs_sz},Bs{1,i+funs_sz},Vs{1,i+funs_sz}] ...
            %    = fun(dataset, decompose_mode.economy);
            %[Us{1,i+funs_sz*2},Bs{1,i+funs_sz*2},Vs{1,i+funs_sz*2}] ...
            %    = fun(dataset, decompose_mode.zero);
        end
        
        for i = 1:funs_sz
            for mode = 1:funs_sz
                if (mode ~= 1) % TODO: Remove once other modes are implemented!
                    continue 
                end
                U = Us{1,i+funs_sz*(mode-1)};
                B = Bs{1,i+funs_sz*(mode-1)};
                V = Vs{1,i+funs_sz*(mode-1)};

                reconstructed = U * B * V';
                assert(all(is_almost_equal(dataset, reconstructed, tol), 'all'));
                
                % test if anything in B except diagonal and superdiagonal is zero
                B_zerod = zeros(size(B),'like',B);
                B_zerod(1:min(m,n),1:min(m,n)) = B(1:min(m,n),1:min(m,n)) - diag(diag(B)) - diag(diag(B,1),1);
                max_norm_B_zerod = max(B_zerod,[],'all');
                disp(['||B-diag(diag(B))-diag(diag(B,1),1)||_max=', num2str(max_norm_B_zerod)]);
                assert(all(is_almost_equal(B_zerod, zeros(size(B)), tol), 'all'));
            end
        end
    end
end
