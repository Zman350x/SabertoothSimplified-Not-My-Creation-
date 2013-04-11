frsky_i2c_receiver
------------------

 - [Used Hardware](#hardware)

Receives FRSKY-CPPM Signals, outputs control signals to Sabertooth Motorcontroler.
Override & Status-Checks through i2c.

### TODO
 - Cutoff interrupt for Sabertooth


<a id="hardware"></a>
Used Hardware
-------------
 - Frsky 2-Way receiver
 - Arduino Pro mini (328)

Software
--------
### i2c Registers
<table class="table table-striped">
  <tr>
    <th>reg</th>
    <th>dir</th>
    <th colspan="2">description</th>
  </tr>
  <tr>
    <td>0x00</td>
    <td align="center">R</td>
    <td colspan="2">Status</td>
  </tr>
  <tr>
    <td colspan="4"><strong>Raw PPM Input</strong></td>
  </tr>
  <tr>
    <td>0x01</td>
    <td align="center">R</td>
    <td>C1 (MSB)</td>
    <td rowspan="2">PPM-Value, 1000-2000ms,<br />
    1500 center</td>
  </tr>
  <tr>
    <td>0x02</td>
    <td align="center">R</td>
    <td>C1 (LSB)</td>
  </tr>
  <tr>
    <td>0x03</td>
    <td align="center">R</td>
    <td>C2 (MSB)</td>
    <td rowspan="2">PPM-Value, 1000-2000ms,<br />
    1500 center</td>
  </tr>
  <tr>
    <td>0x04</td>
    <td align="center">R</td>
    <td>C2 (LSB)</td>
  </tr>
  <tr>
    <td>0x05</td>
    <td align="center">R</td>
    <td>C3 (MSB)</td>
    <td rowspan="2">PPM-Value, 1000-2000ms,<br />
    1500 center</td>
  </tr>
  <tr>
    <td>0x06</td>
    <td align="center">R</td>
    <td>C3 (LSB)</td>
  </tr>
  <tr>
    <td>0x07</td>
    <td align="center">R</td>
    <td>C4 (MSB)</td>
    <td rowspan="2">PPM-Value, 1000-2000ms,<br />
    1500 center</td>
  </tr>
  <tr>
    <td>0x08</td>
    <td align="center">R</td>
    <td>C4 (LSB)</td>
  </tr>
  
  <tr>
    <td>0x09</td>
    <td align="center">R*</td>
    <td>C5 (MSB)</td>
    <td rowspan="2"><strong>reserved</strong><br />always 0</td>
  </tr>
  <tr>
    <td>0x10</td>
    <td align="center">R*</td>
    <td>C5 (LSB)</td>
  </tr>
  
  <tr>
    <td>0x11</td>
    <td align="center">R*</td>
    <td>C6 (MSB)</td>
    <td rowspan="2"><strong>reserved</strong><br />always 0</td>
  </tr>
  <tr>
    <td>0x12</td>
    <td align="center">R*</td>
    <td>C6 (LSB)</td>
  </tr>
  
  
  <tr>
    <td colspan="4"><strong>Output Values</strong></td>
  </tr>
  <tr>
    <td>0x13</td>
    <td align="center">R/W*</td>
    <td>DRIVE</td>
    <td rowspan="2">-127 - +127 (int8 signed)<br />
    Writeable if override set</td>
  </tr>
  <tr>
    <td>0x14</td>
    <td align="center">R/W*</td>
    <td>TURN</td>
  </tr>
  
  <tr>
    <td colspan="4"><strong>Configuration</strong></td>
  </tr>
  <tr>
    <td>0x15</td>
    <td align="center">R/W</td>
    <td>CONFIG</td>
    <td>Configuration for Override etc.</td>
  </tr>    
 
</table>

#### 0x00 Status

<table class="table table-striped">
<tr>
  <th colspan="8">Bits</th>
</tr>
<tr>
  <td>7</td>
  <td>6</td>
  <td>5</td>
  <td>4</td>
  <td>3</td>
  <td>2</td>
  <td>1</td>
  <td>0</td>
</tr>

</table>

#### 0x15 Configuration