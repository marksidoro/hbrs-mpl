#!/usr/bin/env python3

# vim:set tabstop=8 shiftwidth=4 expandtab:
# kate: syntax python; space-indent on; indent-width 4;

# Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

import os
import sys
import logging
from jinja2 import Template
from pathlib import Path
from datetime import datetime

if __name__ == '__main__':
    script_dir = Path(__file__).resolve().parent
    project_dir = script_dir.parent
    categories = ['core', 'detail', 'dt', 'fn' ]
    
    # Terminology
    # top       level: src/hbrs/mpl
    # category  level: src/hbrs/mpl/fn, src/hbrs/mpl/dt
    # component level: src/hbrs/mpl/fn/pca, src/hbrs/mpl/dt/submatrix
    
    # Examples
    # category  level header: src/hbrs/mpl/fn/pca.hpp, src/hbrs/mpl/dt/submatrix.hpp
    # component level header: src/hbrs/mpl/fn/pca/fwd.hpp,  src/hbrs/mpl/dt/submatrix/fwd.hpp,
    #                         src/hbrs/mpl/fn/pca/impl.hpp, src/hbrs/mpl/dt/submatrix/impl.hpp
    
    with open(str(script_dir / 'templates' / 'category_hdr.hpp.j2')) as f:
        tmpl_cat_hdr = Template(f.read())
    
    with open(str(script_dir / 'templates' / 'component_hdr.hpp.j2')) as f:
        tmpl_cmp_hdr = Template(f.read())
    
    with open(str(script_dir / 'templates' / 'CMakeLists.txt.j2')) as f:
        tmpl_cmake = Template(f.read())
    
    for cat in categories:
        cat_dir = project_dir / 'src/hbrs/mpl/' / cat
        
        for cmp_dir in sorted([x for x in cat_dir.iterdir() if x.is_dir()]):
            # Create component impl level CMakeLists.txt
            impl_dir = cmp_dir / 'impl'
            impl_cmake_file = impl_dir / 'CMakeLists.txt'
            if impl_dir.exists() and not impl_cmake_file.exists():
                impl_srcs = sorted([x for x in impl_dir.iterdir() if x.is_file() and x.suffix == '.cpp'])
                if impl_srcs:
                    print(str(impl_cmake_file))
                    with open(str(impl_cmake_file), 'w') as f:
                        f.write(tmpl_cmake.render(
                            category=cat,
                            component=cmp_dir,
                            srcs=impl_srcs,
                            now=datetime.utcnow()
                        ))
            
            # Create component benchmark level CMakeLists.txt
            bench_dir = cmp_dir / 'benchmark'
            bench_cmake_file = bench_dir / 'CMakeLists.txt'
            if bench_dir.exists() and not bench_cmake_file.exists():
                bench_srcs = sorted([x for x in bench_dir.iterdir() if x.is_file() and x.suffix == '.cpp'])
                print(str(bench_cmake_file))
                with open(str(bench_cmake_file), 'w') as f:
                    f.write(tmpl_cmake.render(
                        category=cat,
                        component=cmp_dir,
                        benchmarks=bench_srcs,
                        now=datetime.utcnow()
                    ))
            
            # Create component test level CMakeLists.txt
            test_dir = cmp_dir / 'test'
            test_cmake_file = test_dir / 'CMakeLists.txt'
            if test_dir.exists() and not test_cmake_file.exists():
                test_srcs = sorted([x for x in test_dir.iterdir() if x.is_file() and x.suffix == '.cpp'])
                print(str(test_cmake_file))
                with open(str(test_cmake_file), 'w') as f:
                    f.write(tmpl_cmake.render(
                        category=cat,
                        component=cmp_dir,
                        tests=test_srcs,
                        now=datetime.utcnow()
                    ))
            
            # Create component level CMakeLists.txt
            sub_cmake_file = cmp_dir / 'CMakeLists.txt'
            sub_dirs = sorted([x for x in cmp_dir.iterdir() if x.is_dir() and (x / 'CMakeLists.txt').exists()])
            test_src = cmp_dir / 'test.cpp'
            if not sub_cmake_file.exists() and (sub_dirs or test_src.exists()):
                print(str(sub_cmake_file))
                with open(str(sub_cmake_file), 'w') as f:
                    f.write(tmpl_cmake.render(
                        category=cat,
                        component=cmp_dir,
                        sub_dirs=sub_dirs,
                        test=test_src if test_src.exists() else None,
                        now=datetime.utcnow()
                    ))
            
            for sub in [ 'fwd', 'impl' ]:
                sub_dir = cmp_dir / sub
                sub_hdr_file = cmp_dir / (sub + '.hpp')
                
                # Create component level headers
                if not sub_hdr_file.exists():
                    if sub_dir.exists():
                        sub_hdrs = sorted([x.name for x in sub_dir.iterdir() if x.is_file() and x.suffix == '.hpp'])
                    else:
                        sub_hdrs = []
                    
                    print(str(sub_hdr_file))
                    with open(str(sub_hdr_file), 'w') as f:
                        f.write(tmpl_cmp_hdr.render(
                            category=cat,
                            component=cmp_dir,
                            sub=sub,
                            headers=sub_hdrs,
                            now=datetime.utcnow()
                        ))
           
            
            # Create category level headers
            cmp_hdr_file = cmp_dir.with_suffix(cmp_dir.suffix+'.hpp')
            if not cmp_hdr_file.exists():
                print(str(cmp_hdr_file))
                with open(str(cmp_hdr_file), 'w') as f:
                    f.write(tmpl_cat_hdr.render(
                        category=cat,
                        component=cmp_dir,
                        now=datetime.utcnow()
                    ))
            
        # Create category level CMakeLists.txt
        cat_cmake_file = cat_dir / 'CMakeLists.txt'
        cat_dirs = sorted([x for x in cat_dir.iterdir() if x.is_dir() and (x / 'CMakeLists.txt').exists()])
        if not cat_cmake_file.exists():
            print(str(cat_cmake_file))
            with open(str(cat_cmake_file), 'w') as f:
                f.write(tmpl_cmake.render(
                    category=cat,
                    sub_dirs=cat_dirs,
                    now=datetime.utcnow()
                ))
