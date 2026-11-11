#!/usr/bin/env python3
"""Scalar palette and coloured-cylinder regression tests."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
AUTO = os.environ.get('MOLAUTO', str(ROOT / 'molauto'))
MOL = os.environ.get('MOLSCRIPT', str(ROOT / 'molscript'))


class ScalarTests(unittest.TestCase):
    def run_auto(self, *args):
        return subprocess.run([AUTO, *args, str(ROOT/'examples/ras.pdb')],
                              capture_output=True, text=True)

    def test_options(self):
        for mode in ('-bfactor', '-alphafold', '-alphafold_gradient'):
            for style in ('-nice', '-publication', '-cylinder'):
                result = self.run_auto(mode, style, '-stick')
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn('atomcolour not in waters b-factor', result.stdout)
                self.assertNotIn('rainbow;', result.stdout)
                self.render(result.stdout, '-svg')

    def test_errors(self):
        for args in [('-bfactor_range','0','100'), ('-bfactor','-alphafold'),
                     ('-bfactor','-nocolour'), ('-alphafold','-colourblind'),
                     ('-bfactor','-bfactor_range','nan','100'),
                     ('-bfactor','-bfactor_range','100','0')]:
            result = self.run_auto(*args)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(result.stdout, '')

    def test_ranges_and_nonfinite(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/'values.pdb'
            original = (ROOT/'examples/ras.pdb').read_text().splitlines(True)
            for score in (' 20.00', '   nan', '120.00'):
                path.write_text(''.join(line[:60]+score+line[66:] if line.startswith(('ATOM  ','HETATM')) else line
                                        for line in original))
                mode = '-alphafold' if score == '120.00' else '-bfactor'
                result = subprocess.run([AUTO, mode, str(path)],capture_output=True,text=True)
                if score == '   nan':
                    self.assertNotEqual(result.returncode,0)
                    self.assertEqual(result.stdout,'')
                else:
                    self.assertEqual(result.returncode,0,result.stderr)
                    if score == ' 20.00': self.assertIn('19.5 20.5 blue-white-red',result.stdout)
                    else: self.assertIn('clamping',result.stderr)

    def test_coloured_cylinder_axes(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/'axes.pdb'
            for axis in ((1,0,0),(0,1,0),(0,-1,0),(0,0,1),(1,2,3)):
                path.write_text(''.join(
                    f'ATOM  {i:5d}  CA  ALA A{i:4d}    {i*axis[0]*3.:8.3f}{i*axis[1]*3.:8.3f}{i*axis[2]*3.:8.3f}  1.00{i*20.:6.2f}           C\n'
                    for i in range(1,6))+'END\n')
                scene=f'plot read m "{path}"; transform atom * by centre position atom *; set colourparts on, residuecolour amino-acids b-factor 0 100 alphafold-gradient; cylinder from A1 to A5; end_plot\n'
                for mode in ('-svg','-ps','-x3d','-pov'):
                    self.render(scene,mode)

    def test_residue_and_ligand_range(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/'mixed.pdb'
            records=[]
            def atom(name, residue, chain, number, value, insertion=' '):
                i=len(records)+1
                records.append(f'ATOM  {i:5d} {name:^4s} {residue:>3s} {chain}{number:4d}{insertion}   '
                               f'{i*2.:8.3f}{i%3:8.3f}{0.:8.3f}  1.00{value:6.2f}           C\n')
            for chain in ('A','B'):
                for i in range(1,4):
                    atom('CA','ALA',chain,i,10+i*10,'A' if i==2 else ' ')
                    atom('N','ALA',chain,i,99,'A' if i==2 else ' ')
            atom('P','DA','D',1,60); atom('O','DA','D',1,80)
            atom('C','LIG','L',1,10); atom('O','LIG','L',1,90)
            atom('O','HOH','W',1,999)
            path.write_text(''.join(records)+'END\n')
            for extra, bounds in [((), '10 90'), (('-noligand',), '20 70')]:
                result=subprocess.run([AUTO,'-bfactor',*extra,str(path)],capture_output=True,text=True)
                self.assertEqual(result.returncode,0,result.stderr)
                self.assertIn(f'b-factor {bounds} blue-white-red',result.stdout)
                self.render(result.stdout,'-svg')

    def render(self, scene, mode):
        with tempfile.TemporaryDirectory() as tmp:
            source, target = Path(tmp)/'scene.in', Path(tmp)/'output'
            source.write_text(scene)
            result = subprocess.run([MOL, mode, '-in', str(source), '-out', str(target)],
                                    capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stderr)
            data = target.read_text()
            if mode in ('-svg', '-x3d'): ET.fromstring(data)
            return data

    def test_cylinder_backends(self):
        scene = self.run_auto('-alphafold_gradient', '-cylinder').stdout
        for mode in ('-ps','-svg','-mp','-pov','-vrml','-x3d','-x3dv','-webgl','-raster3d'):
            with self.subTest(mode=mode): self.render(scene, mode)

    def test_exact_atom_colours(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/'scores.pdb'
            values = [-1,0,25,50,50.01,70,70.01,90,90.01,100,101]
            path.write_text(''.join(
                f'HETATM{i:5d}  C   LIG A{i:4d}    {i*3.:8.3f}{0.:8.3f}{0.:8.3f}  1.00{v:6.2f}           C\n'
                for i,v in enumerate(values,1))+'END\n')
            for palette in ('alphafold','alphafold-gradient','blue-white-red'):
                scene = f'plot read m "{path}"; set atomcolour atom * b-factor 0 100 {palette}; cpk atom *; end_plot\n'
                root = ET.fromstring(self.render(scene, '-x3d'))
                # X3D sphere materials are emitted as ProtoInstance fields.
                colours = []
                for sphere in root.findall('.//ProtoInstance[@name="Ball"]'):
                    f = sphere.find('fieldValue[@name="dc"]')
                    colours.append(tuple(map(float, f.get('value').split())) if f is not None else (1,1,1))
                self.assertEqual(len(colours), len(values))
                af = [(238/255,132/255,83/255),(249/255,220/255,77/255),
                      (117/255,199/255,244/255),(33/255,82/255,206/255)]
                for value, actual in zip(values,colours):
                    f = min(100,max(0,value))/100
                    if palette == 'alphafold': expected = af[3 if f>.9 else 2 if f>.7 else 1 if f>.5 else 0]
                    elif palette == 'blue-white-red': expected = (min(1,2*f),min(2*f,2*(1-f)),min(1,2*(1-f)))
                    else:
                        stops=[0,.5,.7,1]; i=0 if f<.5 else 1 if f<.7 else 2
                        t=(f-stops[i])/(stops[i+1]-stops[i])
                        expected=tuple(a+t*(b-a) for a,b in zip(af[i],af[i+1]))
                    for a,b in zip(actual,expected): self.assertAlmostEqual(a,b,places=3)


if __name__ == '__main__': unittest.main(verbosity=2)
