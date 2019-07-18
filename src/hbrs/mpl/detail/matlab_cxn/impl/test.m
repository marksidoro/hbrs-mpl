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

% TODO: replace isequal with is_almost_equal to better handle floating point arithmetics

function test()
    pca_filter_test();
    pca_test();
    svd_test();
    bidiag_test();
end

function pca_filter_test()
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @pca_filter_level0, @pca_filter_level0};
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        [m,n] = size(dataset);
        
        if m-1<n
            p = m-1;
        else
            p = min(m,n);
        end
        
        runs_per_fun = 2+p;
        data = cell(1,funs_sz*runs_per_fun);
        latent = cell(1,funs_sz*runs_per_fun);

        for i = 1:funs_sz
            fun = funs{1,i};
            
            [data{1,i}, latent{1,i}] = ...
                fun(dataset, diag(ones(p)));
            
            [data{1,i+funs_sz}, latent{1,i+funs_sz}] = ...
                fun(dataset, diag(zeros(p)));
            
            for j = 1:p
                filtered = diag(zeros(p));
                filtered(j) = 1;
                [data{1,i+funs_sz*(1+j)}, latent{1,i+funs_sz*(1+j)}] = ...
                    fun(dataset, filtered);
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
            [coeffs{1,i},scores{1,i},latents{1,i}] ...
                = fun(dataset, true);
            [coeffs{1,i+funs_sz},scores{1,i+funs_sz},latents{1,i+funs_sz}] ...
                = fun(dataset, false);
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
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @svd_level0, @svd_level1 };
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        
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

        for i = 1:funs_sz-1
            % TODO: test if anything in S except diagonal is zero
            
            assert(isequal(Us{1,i}, Us{1,i+1}));
            assert(isequal(Ss{1,i}, Ss{1,i+1}));
            assert(isequal(Vs{1,i}, Vs{1,i+1}));

            assert(isequal(Us{1,i+funs_sz}, Us{1,i+funs_sz+1}));
            assert(isequal(Ss{1,i+funs_sz}, Ss{1,i+funs_sz+1}));
            assert(isequal(Vs{1,i+funs_sz}, Vs{1,i+funs_sz+1}));
            
            assert(isequal(Us{1,i+funs_sz*2}, Us{1,i+funs_sz*2+1}));
            assert(isequal(Ss{1,i+funs_sz*2}, Ss{1,i+funs_sz*2+1}));
            assert(isequal(Vs{1,i+funs_sz*2}, Vs{1,i+funs_sz*2+1}));
        end
    end
end

function bidiag_test()
    datasets = samples();
    
    [~, ds_sz ] = size(datasets);
    funs = { @bidiag_level0, };
    [~, funs_sz] = size(funs);
    
    for ds_i = 1:ds_sz
        dataset = datasets{1,ds_i};
        
        Us = cell(1,funs_sz*3);
        Bs = cell(1,funs_sz*3);
        Vs = cell(1,funs_sz*3);

        for i = 1:funs_sz
            fun = funs{1,i};
            [Us{1,i},Bs{1,i},Vs{1,i}] ...
                = fun(dataset, decompose_mode.complete);
            [Us{1,i+funs_sz},Bs{1,i+funs_sz},Vs{1,i+funs_sz}] ...
                = fun(dataset, decompose_mode.economy);
            [Us{1,i+funs_sz*2},Bs{1,i+funs_sz*2},Vs{1,i+funs_sz*2}] ...
                = fun(dataset, decompose_mode.zero);
        end

        for i = 1:funs_sz
            for mode = 0:funs_sz-1
                U = Us{1,i+funs_sz*mode};
                B = Bs{1,i+funs_sz*mode};
                % TODO: test if anything in B except diagonal and superdiagonal is zero
                V = Vs{1,i+funs_sz*mode};

                reconstructed = U * B * V';
                assert(isequal(dataset, reconstructed));
            end
        end
    end
end
