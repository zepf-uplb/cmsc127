using System;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.IO;
using Gtk;
using Sample;
using System.Linq;

public partial class MainWindow: Gtk.Window
{	
	Boolean declare, ok, value, print, printer, varFound, scan, scanner, cnd, and, or, xor, op, an, negate, cmp, comment, execute, comments, win;
	int begin = 0, end = 0, error = 0, i = 0, k = 0;
	String scanned = "", match = "", varname = "", classname = "";
	Hashtable variables = new Hashtable ();
	
	public MainWindow (): base (Gtk.WindowType.Toplevel)
	{
		Build ();
		initTreeView(lex, "Lexeme", "Classification");
		initTreeView (stable, "Identifier", "Value");

	}

	void initTreeView(TreeView t, String label1, String label2){ 
		TreeViewColumn columnA = new TreeViewColumn ( );
		CellRendererText cellA = new CellRendererText ( );
		columnA.FixedWidth = 100;
		columnA.Title = label1;
		columnA.PackStart (cellA, true);
		columnA.AddAttribute (cellA, "text", 0);
		t.AppendColumn (columnA);

		TreeViewColumn columnB = new TreeViewColumn ( );
		CellRendererText cellB = new CellRendererText ( );
		columnB.FixedWidth = 50;
		columnB.Title = label2;
		columnB.PackStart (cellB, true);
		columnB.AddAttribute (cellB, "text", 1);
		t.AppendColumn (columnB);

		ListStore store = new ListStore (typeof (string), typeof (string));
		t.Model = store;

	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}

	protected void OnButton5Clicked (object sender, EventArgs e)
	{
		variables = new Hashtable ();
		variables.Add ("IT", new Variable ("", ""));
		((ListStore) lex.Model).Clear ();
		declare = value = print = printer = varFound = scan = scanner = cnd = and = or = xor = op = an = negate = cmp = comment = win = false;
		ok = execute = true;
		i = error = begin = end = 0;
		console.Buffer.Text = "";
		mainFxn ();
	}

	void mainFxn()
	{
		ListStore lexModel = (ListStore) lex.Model;
		ListStore sTableModel = (ListStore) stable.Model;
		Stack <String> operands = new Stack <String>();

		String[] regEx = {
			@"^\s*HAI\s*", 
			@"^\s*KTHXBYE\s*", 
			@"^\s*I HAS A\s*",
			@"\s*ITZ\s*",
			@"^\s*R\s+",
			@"^\s*VISIBLE\s*", 
			@"^\s*GIMMEH\s*", 
			@"^\s*SUM OF\s*",
			@"^\s*DIFF OF\s*",
			@"^\s*PRODUKT OF\s*",
			@"^\s*QUOSHUNT OF\s*",
			@"^\s*MOD OF\s*",
			@"^\s*AN\s+",
			@"^\s*BTW\s*", 
			@"^\s*OBTW\s*", 
			@"^\s*TLDR\s*",
			@"^\s*BIGGR OF\s*", 
			@"^\s*SMALLR OF\s*", 
			@"^\s*BOTH OF\s*", 
			@"^\s*EITHER OF\s*", 
			@"^\s*WON OF\s*", 
			@"^\s*NOT", 
			@"^\s*ALL OF\s*",
			@"^\s*ANY OF\s*", 
			@"^\s*BOTH SAEM\s*",
			@"^\s*DIFFRINT\s*",
			@"^\s*O RLY\?", 
			@"^\s*YA RLY,?\s*", 
			@"^\s*NO WAI,?\s*", 
			@"^\s*OIC\s*",
			@"^\s*WTF\?\s*", 
			@"^\s*OMG\s+", 
			@"^\s*OMGWTF\s+", 
			@"^\s*GTFO",
			@"^\s*-?[0-9]+\.[0-9]*\s*",
			@"^\s*-?[0-9]+\s*",
			@"^\s*(WIN|FAIL)\s*",
			@"^[a-z|A-Z|_][a-z|A-Z|0-9]*\s*",
			"\".+\"",
			@".+"
		};

		String[] classification = {
			"START CODE DELIMITER",
			"END CODE DELIMITER",
			"VARIABLE DECLARATION", 
			"INITIALIZATION",
			"ASSIGNMENT",
			"OUTPUT",
			"INPUT",
			"ARITHMETIC OPERATION", 
			"ARITHMETIC OPERATION", 
			"ARITHMETIC OPERATION", 
			"ARITHMETIC OPERATION", 
			"ARITHMETIC OPERATION",
			"NOISE WORD", 
			"SINGLE LINE COMMENT", 
			"START BLOCK COMMENT DELIMITER", 
			"END BLOCK COMMENT DELIMITER",
			"BINARY OPERATION",
			"BINARY OPERATION",
			"BOOL",
			"BOOL", 
			"BOOL", 
			"NEGATE",
			"BOOL", 
			"BOOL", 
			"COMPARISON", 
			"COMPARISON",
			"START CONTROL STATEMENTS", 
			"WIN CONTROL STATEMENTS", 
			"FAIL CONTROL STATEMENTS", 
			"END CONTROL STATEMENTS", 
			"CONTROL STATEMENTS", 
			"CONTROL STATEMENTS", 
			"CONTROL STATEMENTS",
			"CONTROL STATEMENTS",
			"NUMBAR LITERAL",
			"NUMBR LITERAL",
			"TROOF LITERAL",
			"VARIABLE IDENTIFIER",
			"YARN LITERAL",
			"ERROR"
		};

		String[] reserved = {
			"HAI", 
			"KTHXBYE", 
			"A",
			"ITZ",
			"R",
			"VISIBLE", 
			"GIMMEH", 
			"BTW", 
			"OBTW", 
			"TLDR",
			"NOT", 
			"DIFFRINT",
			"OIC",
			"OMG", 
			"OMGWTF", 
			"GTFO"
		};

		String[] opSymbols = {
			"+",
			"-",
			"*",
			"/",
			"%",
			"<",
			">",
			"&&",
			"||",
			"!=",
			"==",
			"!"
		};

		String s = textview5.Buffer.Text;
		String[] code = s.TrimEnd('\n').Split('\n');

		for(; i < code.Length && !scanner; i++){

			String line = code [i].Trim(' ');
			LinkedList <String> cond = new LinkedList<String> ();
			cnd = false;

			while (!(line.Equals("")) && !scanner) {

				for (k = 0; k < regEx.Length && !scanner; k++) {

					Match m = Regex.Match (line, regEx [k]);
					match = m.Value.Trim(' ');

					if (m.Success) {

						if (classification [k].Equals ("END BLOCK COMMENT DELIMITER")) {
							execute = true;
						} 

						if ( classification [k].Equals ("FAIL CONTROL STATEMENTS")) {
							if (win)
								execute = false;
							else
								execute = true;
						} 

						if (classification [k].Equals ("END CONTROL STATEMENTS")) {
							execute = true;
						} 

						if (execute) {
							classname = classification [k];
							if (classification [k].Equals ("SINGLE LINE COMMENT")) {
								comment = true;
								execute = false;
							} 
							if (classification [k].Equals ("START BLOCK COMMENT DELIMITER")) {
								execute = false;
							} 
							if (classification [k].Equals ("START CONTROL STATEMENTS")) {
								if (((Variable)variables ["IT"]).getVal () == "WIN")
									win = true;
								else
									execute = false;
							} 
							if (i == code.Length - 1 && !classification [k].Equals ("END CODE DELIMITER"))
								error++;
							if (classification [k].Equals ("ERROR")) {
								error++;
								console.Buffer.Text += "ERROR";
								ok = false;
							}
							if (classification [k].Equals ("NOISE WORD"))
								an = true;
							if (classification [k].Equals ("NEGATE")) {
								if (cnd || cmp || op) {
									op = true;
									operands.Push ("!");
								} else
									negate = !negate;
							}
							if (scan && !classification [k].Equals ("VARIABLE IDENTIFIER")) {
								scanner = true;
								scan = false;
								varname = match;
							}
							if (classification [k].Equals ("START CODE DELIMITER")) {
								begin++;
								if (begin > 1)
									ok = false;
							}
							if (classification [k].Equals ("END CODE DELIMITER")) {
								end++;
								if (end > 1)
									ok = false;
							}
							if (classification [k].Equals ("OUTPUT")) {
								print = true;
							}
							if (classification [k].Equals ("INPUT")) {
								scan = true;
							}
							if (classification [k].Equals ("ARITHMETIC OPERATION")) {
								op = true;
								an = !an;
								if (match.Equals ("SUM OF"))
									operands.Push ("+");
								if (match.Equals ("DIFF OF"))
									operands.Push ("-");
								if (match.Equals ("PRODUKT OF"))
									operands.Push ("*");
								if (match.Equals ("QUOSHUNT OF"))
									operands.Push ("/");
								if (match.Equals ("MOD OF"))
									operands.Push ("%");
							}
							if (classification [k].Equals ("COMPARISON")) {
								op = true;
								cmp = true;
								if (match.Equals ("BOTH SAEM"))
									operands.Push ("==");
								if (match.Equals ("DIFFRINT"))
									operands.Push ("!=");
							}
							if (classification [k].Equals ("BOOL")) {
								cnd = true;
								op = true;
								if (match.Equals ("BOTH OF") || match.Equals ("ALL OF")) {
									and = true;
								}
								if (match.Equals ("EITHER OF") || match.Equals ("ANY OF")) {
									or = true;
								}
								if (match.Equals ("WON OF")) {
									xor = true;
								}
							}
							if (classification [k].Equals ("ASSIGNMENT")) {
								if (varFound) {
									value = true;
									varFound = false;
								} else {
									continue;
								}
							}
							if (classification [k].Equals ("VARIABLE DECLARATION")) {
								declare = true;
							}
							if (classification [k].Equals ("INITIALIZATION")) {
								if (varFound) {
									value = true;
									varFound = false;
								} else
									continue;
							}
							if (classification [k].Equals ("BINARY OPERATION")) {
								op = true;
								if (match.Equals ("BIGGR OF"))
									operands.Push (">");
								if (match.Equals ("SMALLR OF"))
									operands.Push ("<");
							}
							if (classification [k].Equals ("VARIABLE IDENTIFIER")) {
								if (declare) {
									if (reserved.Contains (m.Value)) {
										error++;
										ok = false;
									}
									if (variables.ContainsKey (match)) {
										error++;
									} else {
										variables.Add (match, new Variable ("", ""));
										varFound = true;
										varname = match;
									}
									declare = false;
								} else {
									if (variables.ContainsKey (match)) {
										if (scan) {
											scanner = true;
											Submit.Visible = true;
											inputer.Editable = true;
											inputer.GrabFocus ();
											scan = false;
											varname = match;
										} else if (op) {
											an = false;
											String temp = ((Variable)variables [match]).getVal ();
											if (temp == "") {
												error++;
												break;
											} else
												operands.Push (temp);
										} else {
											varFound = true;
											varname = match;
										}
									} else {
										error++;
									}
								}
							}
							if (classification [k].Equals ("YARN LITERAL")) {
								match = match.Trim ('\"');
								lexModel.AppendValues ("\"", "STRING DELIMITER");
								lexModel.AppendValues (match, classification [k]);
								lexModel.AppendValues ("\"", "STRING DELIMITER");
								if (printer)
									printFxn (classification [k], match, variables);
								else if (value) {
									((Variable)variables [varname]).setter ("YARN", match);
								}
								ok = false;
							}
							if ((classification [k].Equals ("NUMBR LITERAL") || classification [k].Equals ("NUMBAR LITERAL"))) {
								if (printer)
									printFxn (classification [k], match, variables);
								if (op) {
									operands.Push (match);
									an = false;
								} else if (value) {
									((Variable)variables [varname]).setter ("NUMBR", match);
								}
							}
							if ((classification [k].Equals ("TROOF LITERAL"))) {
								if (value) {
									((Variable)variables [varname]).setter ("TROOF", match);
								}
							}
							if (ok) {
								lexModel.AppendValues (match, classification [k]);
							}
							if (printer && varFound)
								printFxn (classification [k], match, variables);
							if (print) {
								printer = true;
								print = false;
							}
						}

						int start = line.IndexOf (m.Value);
						line = line.Remove (start, m.Value.Length);
						ok = true;
						k = 0;
					}//end of if(m.Success)
				}//end of for loop

				if (comment) {
					comment = false;
					execute = true;
					break;
				}
				if (varFound) {
					((Variable)variables ["IT"]).setter (((Variable)variables [varname]).getDataType (), ((Variable) variables [varname]).getVal());
					varFound = false;
				}
				if (op && an) {
					error++;
					console.Buffer.Text += "OPERATOR MISMATCH" + "\n";
				}
				if (!noError(i)) break;
				if (op) {
					Stack<float> st = new Stack<float> ();
					while (operands.Count != 0) {
						String str = operands.Pop ();
						if (opSymbols.Contains(str)) {
							float x = st.Pop ();
							float y = str.Equals("!")? 0 : st.Pop ();
							if (str.Equals ("+"))
								st.Push (x + y);
							else if (str.Equals ("-"))
								st.Push (x - y);
							else if (str.Equals ("*"))
								st.Push (x * y);
							else if (str.Equals ("/"))
								st.Push (x / y);
							else if (str.Equals ("%"))
								st.Push (x % y);
							else if (str.Equals (">"))
								st.Push (y > x ? y : x);
							else if (str.Equals ("<"))
								st.Push (y < x ? y : x);
							else if (str.Equals ("=="))
								st.Push (x == y ? 1 : 0);
							else if (str.Equals ("!="))
								st.Push (x != y ? 1 : 0);
							else if (str.Equals ("!")) {
								st.Push (x == 1? 0 : 1);
							}
						} else { 
							try{
								if(str.Equals("WIN")) st.Push(1);
								else if(str.Equals("FAIL")) st.Push(0);
								else st.Push (float.Parse (str));
							}catch(Exception){
								error++; 
								break;
							}
						}
					}
					String result = "";
					if (cnd) {
						while (st.Count != 0) cond.AddLast (st.Pop () == 1 ? "WIN" : "FAIL");
						if (and) {
							result = cond.Contains ("FAIL") ? "FAIL" : "WIN";
						} else if (or) {
							result = cond.Contains ("WIN") ? "WIN" : "FAIL";
						} else if (xor) {
							result = cond.First != cond.Last ? "WIN" : "FAIL";
						}
					} else {
						float sum = st.Pop ();
						result = cmp ? sum == 1 ? "WIN" : "FAIL" : sum.ToString ();
					}
					if (negate)
						result = result.Equals ("WIN")? "FAIL" : "WIN";
					if (printer) {
						printFxn ("", result, variables);
					} else if (value) {
						((Variable)variables [varname]).setter ("NUMBR", result);
					} else {
						((Variable)variables ["IT"]).setter ("NUMBR", result);
					}
					op = and = or = xor = value = cmp = false;
					cond.Clear();
				}
			}
			if (!noError(i)) break;
			updateSymbolTable (sTableModel, variables);
		}
	}

	Boolean noError(int N)
	{
		if(error > 0) {
			console.Buffer.Text = "ERROR FOUND at line " + (N+1);
			return false;
		}
		if(begin > 1 || end > 1) {
			console.Buffer.Text = "ERROR FOUND at line " + (N+1);
			return false;
		}
		if(begin == 0) {
			console.Buffer.Text = "ERROR FOUND at line " + (N+1);
			return false;
		}
		return true;
	}

	void updateSymbolTable(ListStore ls, Hashtable hs)
	{
		ls.Clear ();
		foreach(String name in hs.Keys){
			ls.AppendValues (name, ((Variable) hs [name]).getVal());
		}
	}

	void printFxn(String cla, String val, Hashtable hs)
	{
		if (cla.Equals ("VARIABLE IDENTIFIER")) {
			console.Buffer.Text += ((Variable) hs [val]).getVal();
		}
		else{
			console.Buffer.Text += val;
		}
		console.Buffer.Text += "\n";
		printer = false;
		varFound = false;
	}

	protected void OnFilechooserbutton1SelectionChanged (object sender, EventArgs e)
	{
		string fileAsString = string.Empty;
		string fname = filechooserbutton1.Filename;
		Regex fnamechecker = new Regex (@".lol$");

		if(fnamechecker.IsMatch(fname)){
			using (StreamReader file = File.OpenText(filechooserbutton1.Filename)){
				fileAsString = file.ReadToEnd();
			}
		Gtk.TextBuffer buffer;
		buffer = textview5.Buffer;
		buffer.Text = fileAsString;
		}
	}

	protected void OnSubmitClicked (object sender, EventArgs e)
	{
		scanned = inputer.Buffer.Text;
		scanner = false;
		((Variable)variables [varname]).setter (classname, scanned);
		inputer.Editable = false;
		inputer.Buffer.Text = "";
		Submit.Visible = false;
		mainFxn ();

	}
}
