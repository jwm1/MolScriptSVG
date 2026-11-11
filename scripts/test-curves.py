#!/usr/bin/env python3
"""Native band caps, smooth tubes, spline export and vector curve regressions."""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
MOL = os.environ.get('MOLSCRIPT',str(ROOT/'molscript'))
AUTO = os.environ.get('MOLAUTO',str(ROOT/'molauto'))

class Curves(unittest.TestCase):
    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path=Path(self.tmp.name)

    def render(self,scene,mode):
        source=self.path/'scene.in'; target=self.path/('scene.'+mode[1:])
        source.write_text(scene)
        p=subprocess.run([MOL,'-s',mode,'-in',str(source),'-out',str(target)],
                         capture_output=True,text=True,timeout=30)
        self.assertEqual(p.returncode,0,p.stderr)
        return target

    def scene(self,style=''):
        return f'plot read m "{ROOT / "examples/ras.pdb"}"; transform atom * by centre position atom *; set colourparts on, residuecolour amino-acids b-factor 0 100 alphafold-gradient; {style} coil amino-acids; end_plot\n'

    def test_native_cylinder_caps(self):
        scene=subprocess.check_output([AUTO,'-cylinder','-alphafold_gradient',str(ROOT/'examples/ras.pdb')],text=True)
        root=ET.parse(self.render(scene,'-x3d')).getroot()
        self.assertIsNotNone(root.find('.//ProtoDeclare[@name="Cyl"]//Cylinder'))
        bands=root.findall('.//ProtoInstance[@name="Cyl"]')
        helices=len(re.findall(r'^\s*cylinder from',scene,re.M))
        self.assertGreater(len(bands),helices)
        for field in ('tc','bc'):
            caps=sum(b.find(f'fieldValue[@name="{field}"]') is None for b in bands)
            self.assertEqual(caps,helices)
        if shutil.which('view3dscene'):
            p=subprocess.run(['view3dscene','--write',str(self.path/'scene.x3d')],capture_output=True,text=True,timeout=30,
                             env=dict(os.environ,HOME=str(self.path),XDG_CONFIG_HOME=str(self.path)))
            self.assertEqual(p.returncode,0,p.stderr)
        pov=self.render(scene,'-pov').read_text()
        self.assertIn(' open ',pov)
        self.assertIn('disc {',pov)

    def test_round_tube_normals(self):
        root=ET.parse(self.render(self.scene(),'-x3d')).getroot()
        normals=root.findall('.//Normal')
        self.assertTrue(normals)
        values=list(map(float,normals[0].get('vector').replace(',',' ').split()))
        distinct=set(tuple(round(v,2) for v in values[i:i+3]) for i in range(0,len(values),3))
        self.assertGreater(len(distinct),8)
        for i in range(0,len(values),3):
            self.assertAlmostEqual(sum(v*v for v in values[i:i+3]),1,delta=.02)

    def test_spline_pov(self):
        target=self.render(self.scene(),'-pov')
        data=target.read_text()
        self.assertIn('sphere_sweep { cubic_spline',data)
        self.assertIn('clipped_by',data)
        if shutil.which('povray'):
            p=subprocess.run(['povray','+I'+str(target),'+O'+str(self.path/'pov.png'),
                              '+W80','+H80','+FN','-D'],capture_output=True,text=True,timeout=60,
                             env=dict(os.environ,HOME=str(self.path)))
            self.assertEqual(p.returncode,0,p.stderr[-2000:])

    def test_vector_beziers(self):
        for mode,token in [('-svg',' C '),('-ps','curveto stroke'),('-mp','..controls ')]:
            target=self.render(self.scene('set coilradius 0;'),mode)
            self.assertIn(token,target.read_text())
            if mode=='-svg': ET.parse(target)
            if mode=='-ps' and shutil.which('gs'):
                subprocess.run(['gs','-q','-dBATCH','-dNOPAUSE','-sDEVICE=nullpage',str(target)],check=True,timeout=30)
            if mode=='-mp' and shutil.which('mpost'):
                p=subprocess.run(['mpost','-interaction=nonstopmode',str(target)],cwd=self.path,capture_output=True,text=True,timeout=30)
                self.assertEqual(p.returncode,0,p.stdout[-2000:])
        straight='plot line 0 0 0 to 3 2 0; end_plot\n'
        self.assertNotIn(' C ',self.render(straight,'-svg').read_text())

if __name__=='__main__': unittest.main(verbosity=2)
