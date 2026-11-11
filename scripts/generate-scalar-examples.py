#!/usr/bin/env python3
"""Create explicitly synthetic confidence examples, plus real Ras B-factors."""
from pathlib import Path
import subprocess
import cairosvg

root = Path(__file__).resolve().parents[1]
examples = root/'examples'
lines = (examples/'ras.pdb').read_text().splitlines(True)
keys = list(dict.fromkeys(line[21:27] for line in lines if line.startswith(('ATOM  ','HETATM'))))
scores = {key: 100*i/max(1,len(keys)-1) for i,key in enumerate(keys)}
output = ['REMARK   SYNTHETIC CONFIDENCE DEMONSTRATION; NOT AN ALPHAFOLD PREDICTION\n']
for line in lines:
    if line.startswith(('ATOM  ','HETATM')):
        line = line[:60]+f'{scores[line[21:27]]:6.2f}'+line[66:]
    output.append(line)
(examples/'ras_synthetic_confidence.pdb').write_text(''.join(output))
for name, args, pdb in [
    ('ras_bfactor',['-bfactor'],'ras.pdb'),
    ('ras_confidence',['-alphafold'],'ras_synthetic_confidence.pdb'),
    ('ras_confidence_gradient',['-alphafold_gradient'],'ras_synthetic_confidence.pdb'),
    ('ras_confidence_cylinders',['-alphafold_gradient','-cylinder'],'ras_synthetic_confidence.pdb'),
    ('ras_bfactor_legend',['-bfactor','-legend','-16','-30','0','32','1.5'],'ras.pdb'),
    ('ras_confidence_legend',['-alphafold','-legend','-16','-30','0','32','1.5'],'ras_synthetic_confidence.pdb'),
    ('ras_confidence_gradient_legend',['-alphafold_gradient','-legend','-16','-30','0','32','1.5'],'ras_synthetic_confidence.pdb')]:
    scene = subprocess.check_output([str(root/'molauto'),'-nice','-stick',*args,pdb],cwd=examples)
    (examples/f'{name}.in').write_bytes(scene)
    for mode, ext in [('-svg','svg'),('-ps','ps'),('-mp','mp'),('-pov','pov'),
                      ('-vrml','wrl'),('-x3d','x3d'),('-x3dv','x3dv'),('-webgl','webgl.html')]:
        subprocess.run([str(root/'molscript'),'-s',mode,'-in',f'{name}.in','-out',f'{name}.{ext}'],
                       cwd=examples,check=True)
    cairosvg.svg2png(url=str(examples/f'{name}.svg'),write_to=str(examples/f'{name}_svg.png'))
    print(name)
