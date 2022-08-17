<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: table2.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: generates a text table appropriate for e-mails  ex.  $table =   |
 | new table(  new th(new td("Time", "center"), new td("Student", "center")),   |
 | new tr(new td(" 9:00 am", "center"), new td("Stephen McGrath", "left")),     |
 | new tr(new td("BREAK", "center", 2)) 	);  echo($table->display());           |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 |                                                                              |
 +------------------------------------------------------------------------------+

*/

class table{
	var $rows;
	var $column_widths;
	var $layed_out=false;
	
	function table()
	{
		$rows = func_get_args();
		$this->rows = $rows;
	}
	
	function layout()
	{
		$colspan = 1;
		$layed_out = false;
		while (!$layed_out)
		{
			$layed_out = true;
			$num_rows = count($this->rows);
			for ($i = 0; $i < $num_rows; $i++)
			{
				$row = &$this->rows[$i];
				$num_cells = count($row->cells);
				for ($j = 0; $j < $num_cells; $j++)
				{
					$cell = &$row->cells[$j];
					if (!$cell->layed_out)
					{
						$layed_out = false;
						if ($colspan == $cell->colspan)
						{
							for ($k = $cell->colstart, $width = 0; $k <= $cell->colend; $k++)
							{
								$width += $this->column_widths[$k];
							}
							$k--;
							
							if ($cell->content_length > $width)
							{
								$this->column_widths[$k] += $cell->content_length - $width;
							}
							$cell->layed_out = true;
						}
					}
				}
			}
			$colspan++;
		}
		$this->layed_out = true;
	}

	function separator()
	{
		$output .= "+";
		for ($k = 0; $k < count($this->column_widths); $k++)
		{
			$output .= str_pad("", $this->column_widths[$k] + 2, "-") . "+";
		}
		$output .= "\n";
		return $output;
	}
	
	function display()
	{
		if (!$this->layed_out) { $this->layout(); }
		
		$output .= $this->separator();
		
		$rows = count($this->rows);
		for ($i = 0; $i < $rows; $i++)
		{
			$output .= "| ";
			$row = &$this->rows[$i];
			$cells = count($row->cells);
			for ($j = 0; $j < $cells; $j++)
			{
				$cell = &$row->cells[$j];
				$width = 0;
				for ($k = $cell->colstart; $k <= $cell->colend; $k++)
				{
					$width += $this->column_widths[$k];
				}
				$width += ($cell->colend - $cell->colstart) * 3;
				switch($cell->align)
				{
					case "center":
						$pad = STR_PAD_BOTH;
						break;
						
					case "right":
						$pad = STR_PAD_LEFT;
						break;
						
					default:
						$pad = STR_PAD_RIGHT;
						break;					
				}
				$output .= str_pad($cell->content, $width, " ", $pad);
				$output .= " | ";
			}
			
			if ($cell->colend < sizeof($this->column_widths))
			{
				for ($j = $cell->colend + 1; $j < sizeof($this->column_widths); $j++)
				{
					$output .= str_pad("", $this->column_widths[$j], " ", STR_PAD_RIGHT) . " | ";
				}
			}
			
			$output .= "\n";
			if ($row->th && $i < $rows - 1) { $output .= $this->separator(); }
		}	
		$output .= $this->separator();
		return $output;
	}
}


class th extends tr
{
	function th()
	{
		$args = func_get_args();
		for ($i = 0; $i < sizeof($args); $i++) { $arg_list .= "\$args[$i],"; }
		$arg_list = substr($arg_list, 0, strlen($arg_list) - 1);
		eval("\$this->tr(" . $arg_list . ");");
		$this->th=true;
	}
}

class tr
{
	var $cells;
	var $columns;
	var $th=false;
	
	function tr()
	{
		$cells = func_get_args();
		$num_cells = count($cells);
		for ($i = 0, $column = 0; $i < $num_cells; $i++)
		{
			$cells[$i]->colstart = $column;
			$column += $cells[$i]->colspan;
			$cells[$i]->colend = $column - 1;
		}
		$this->columns = $column;
		$this->cells = $cells;
	}
}

class td
{
	var $colspan;
	var $align;
	var $content;
	var $content_length;
	var $colstart;
	var $colend;
	var $layed_out = false;
	
	function td($content, $align="left", $colspan=1)
	{
		$this->content = $content;
		$this->align = $align;
		if ($colspan == 0) { $colspan = 1; }
		$this->colspan = $colspan;
		$this->content_length = strlen($content);
	}
}

?>
