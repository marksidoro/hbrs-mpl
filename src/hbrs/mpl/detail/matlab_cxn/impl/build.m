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

function build(bin_dir, src_dir, build_type)
    cfg = coder.config('lib');
    
    flags = {'-o', 'matlab_cxn', '-config', cfg, '-d', bin_dir, '-report' };
    
    if strcmpi(build_type, 'Debug')
        cfg.BuildConfiguration = 'Debug';
        flags = {flags{:}, '-g'};
    else
        cfg.BuildConfiguration = 'Faster Runs';
    end
    
    if exist(fullfile(cd, 'lapack_callback.m'), 'file')  == 2
        cfg.CustomLAPACKCallback = 'lapack_callback';
    end
    
    flags = { flags{:}, '-I', src_dir, '-I', bin_dir };
    
    flags = { flags{:}, 'noop'};
    
    matrix_double_type = coder.typeof(double(0), [Inf Inf]);
    matrix_complex_double_type = coder.typeof(complex(double(0), double(0)), [Inf Inf]);
    matrix_dim_type = coder.newtype('matrix_dim');
    double_type = coder.typeof(double(0));
    complex_double_type = coder.typeof(complex(double(0), double(0)));
    uint64_type = coder.typeof(uint64(0));
    bool_type = coder.typeof(logical(true));
    decompose_mode_type = coder.newtype('decompose_mode');
    vector_bool_type = coder.typeof(logical(false), [Inf 1]);
    vector_double_type = coder.typeof(double(0), [Inf 1]);
    vector_complex_double_type = coder.typeof(complex(double(0),double(0)), [Inf 1]);
    vector_index_type = coder.typeof(uint64(1));
    matrix_index_type = coder.typeof(struct('m',uint64(1),'n', uint64(1)));
    matrix_index_type = coder.cstructname(matrix_index_type ,'emxArrayIndex');
    
    for fun = {'bidiag_level0' }
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type, decompose_mode_type} 
        };
    end
    
    for fun = {'svd_level0', 'svd_level1' }
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type, decompose_mode_type}
        };
    end
    
    for fun = {'pca_level0', 'pca_level1', 'pca_level2'}
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type, bool_type, bool_type, bool_type}
        };
    end
    
    for fun = {'pca_filter_level0'}
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type, vector_bool_type, bool_type, bool_type, bool_type} ...
        };
    end
    
    for fun = {'dmd_level1'}
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type, matrix_double_type, uint64_type}
        };
    end
    
    for fun = {'eig_level0'}
        flags = {flags{:}, fun{1}, '-args', ...
            {matrix_double_type}
        };
    end
    
    flags = { flags{:}, 'complex_vdd', '-args', {vector_double_type, double_type}};
    flags = { flags{:}, 'complex_vdvd', '-args', {vector_double_type, vector_double_type}};
    flags = { flags{:}, 'absolute_cd', '-args', {complex_double_type}};
    flags = { flags{:}, 'transpose_md', '-args', {matrix_double_type}};
    flags = { flags{:}, 'mean_md', '-args', {matrix_double_type, matrix_dim_type}};
    flags = { flags{:}, 'variance_md', '-args', {matrix_double_type, double_type, matrix_dim_type}};
    flags = { flags{:}, 'multiply_mdmd', '-args', {matrix_double_type, matrix_double_type}};
    flags = { flags{:}, 'multiply_mdmcd', '-args', {matrix_double_type, matrix_complex_double_type}};
    flags = { flags{:}, 'multiply_mcdmcd', '-args', {matrix_complex_double_type, matrix_complex_double_type}};
    flags = { flags{:}, 'multiply_mdvd', '-args', {matrix_double_type, vector_double_type}};
    flags = { flags{:}, 'multiply_mcdvcd', '-args', {matrix_complex_double_type, vector_complex_double_type}};
    flags = { flags{:}, 'mldivide_mdvd', '-args', {matrix_double_type, vector_double_type}};
    flags = { flags{:}, 'mldivide_mcdvcd', '-args', {matrix_complex_double_type, vector_complex_double_type}};
    flags = { flags{:}, 'diag_vd', '-args', {vector_double_type}};
    flags = { flags{:}, 'diag_vcd', '-args', {vector_complex_double_type}};
    flags = { flags{:}, 'plus_mdmd', '-args', {matrix_double_type, matrix_double_type}};
    flags = { flags{:}, 'plus_mdsd', '-args', {matrix_double_type, double_type}};
    
    flags = { flags{:}, 'at', '-args', {matrix_double_type, matrix_index_type}};
    flags = { flags{:}, 'samples'};
    flags = { flags{:}, 'debug1', '-args', {matrix_double_type}};
    flags = { flags{:}, 'debug2', '-args', {matrix_double_type}};
    
    codegen(flags{:});
end
