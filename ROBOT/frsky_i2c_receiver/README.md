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
    <td>CONFIG1</td>
    <td>Configuration for Override etc.</td>
  </tr>
  <tr>
    <td>0x16</td>
    <td align="center">R/W</td>
    <td>CONFIG2</td>
    <td>Configuration for Override etc.</td>
  </tr>
  <tr>
    <td>0x17</td>
    <td align="center">R/W</td>
    <td>RCMAPPING</td>
    <td>Mapping of RC input to Sabertooh</td>
  </tr>
  
 
</table>

#### 0x00 Status

<table class="table table-striped">
<tr>
  <th colspan="8">Bits</th>
</tr>
<tr>
  <th>7</th>
  <th>6</th>
  <th>5</th>
  <th>4</th>
  <th>3</th>
  <th>2</th>
  <th>1</th>
  <th>0</th>
</tr>
<tr>
  <td>x</td>
  <td>x</td>
  <td>x</td>
  <td>x</td>
  <td>x</td>
  <td><strong>OVER</strong></td>
  <td><strong>DEBUG</strong></td>
  <td><strong>STATE</strong></td>
</tr>
</table>

###### STATE
<table class="table table-striped">
  <tr>
    <th colspan="2">STATE</th>
  </tr>
  <tr>
    <td><strong>BIT 0</strong></td>
    <td>Desc</td>
  </tr>
  <tr>
    <td>0</td>
    <td>INIT</td>
  </tr>
  <tr>
    <td>1</td>
    <td>RUNNING</td>
  </tr>
</table>

###### DEBUG
<table class="table table-striped">
  <tr>
    <th colspan="2">DEBUG</th>
  </tr>
  <tr>
    <td><strong>BIT 1</strong></td>
    <td>Desc</td>
  </tr>
  <tr>
    <td>0</td>
    <td>Debugmode off</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Debugmode on</td>
  </tr>
</table>
No output of Sabertooth Commands in debugmode.

###### OVER
Override of RC input
<table class="table table-striped">
  <tr>
    <th colspan="2">OVER</th>
  </tr>
  <tr>
    <td><strong>BIT 2</strong></td>
    <td>Desc</td>
  </tr>
  <tr>
    <td>0</td>
    <td>Override off</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Override on</td>
  </tr>
</table>
#### 0x15 CONFIG1
<table class="table table-striped">
<tr>
  <th colspan="8">Bits</th>
</tr>
<tr>
  <th>7</th>
  <th>6</th>
  <th>5</th>
  <th>4</th>
  <th>3</th>
  <th>2</th>
  <th>1</th>
  <th>0</th>
</tr>
<tr>
  <td>-</td>
  <td>-</td>
  <td>-</td>
  <td>-</td>
  <td>-</td>
  <td>-</td>
  <td><strong>DEBUG</strong></td>
  <td><strong>STATE</strong></td>
</tr>
</table>
#### 0x16 CONFIG2

#### 0x17 RCMAPPING
<table class="table table-striped">
<tr>
  <th colspan="8">Bits</th>
</tr>
<tr>
  <th>7</th>
  <th>6</th>
  <th>5</th>
  <th>4</th>
  <th>3</th>
  <th>2</th>
  <th>1</th>
  <th>0</th>
</tr>
<tr>
  <td>-</td>
  <td>-</td>
  <td colspan="3" align="center"><strong>TURN</strong></td>
  <td colspan="3" align="center"><strong>DRIVE</strong></td>
</tr>
</table>

<table class="table table-striped">
  <tr>
    <th colspan="4">DRIVE</th>
  </tr>
  <tr>
    <td><strong>BIT 2</strong></td>
    <td><strong>BIT 1</strong></td>
    <td><strong>BIT 0</strong></td>
    <td>Desc</td>
  </tr>
  <tr>
    <td>0</td>
    <td>0</td>
    <td>1</td>
    <td>Channel 1 (C1)</td>
  </tr>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>0</td>
    <td>Channel 2 (C2)</td>
  </tr>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>1</td>
    <td>Channel 3 (C3)</td>
  </tr>
  <tr>
    <td>1</td>
    <td>0</td>
    <td>0</td>
    <td>Channel 4 (C4)</td>
  </tr>
  <tr>
    <td>1</td>
    <td>0</td>
    <td>1</td>
    <td>Channel 5 (C5)*</td>
  </tr>
  <tr>
    <td>1</td>
    <td>1</td>
    <td>0</td>
    <td>Channel 6 (C6)*</td>
  </tr>
</table>

<table class="table table-striped">
  <tr>
    <th colspan="4">TURN</th>
  </tr>
  <tr>
    <td><strong>BIT 5</strong></td>
    <td><strong>BIT 4</strong></td>
    <td><strong>BIT 3</strong></td>
    <td>Desc</td>
  </tr>
  <tr>
    <td>0</td>
    <td>0</td>
    <td>1</td>
    <td>Channel 1 (C1)</td>
  </tr>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>0</td>
    <td>Channel 2 (C2)</td>
  </tr>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>1</td>
    <td>Channel 3 (C3)</td>
  </tr>
  <tr>
    <td>1</td>
    <td>0</td>
    <td>0</td>
    <td>Channel 4 (C4)</td>
  </tr>
  <tr>
    <td>1</td>
    <td>0</td>
    <td>1</td>
    <td>Channel 5 (C5)*</td>
  </tr>
  <tr>
    <td>1</td>
    <td>1</td>
    <td>0</td>
    <td>Channel 6 (C6)*</td>
  </tr>
</table>