

struct InputFileReader
{  
  virtual bool failed_open() = 0;
  virtual string get_string() = 0;
  virtual int read_num() = 0;
  virtual char peek_char() = 0;
  
  /// Check if the next character from @infile is @sym.
  virtual void check_sym(char sym) = 0;
  virtual string getline() = 0;
  
  /// Cleans rubbish off start of string.
  virtual void clean_string(string& s) = 0;
  
  virtual string getline(char deliminator) = 0;
  virtual char get_char() = 0;
  virtual bool eof() = 0;
  virtual void putback(char c) = 0;
};

template<typename StreamType>
struct ConcreteFileReader : public InputFileReader
{
  StreamType infile;
  
  template<typename InputType>
  ConcreteFileReader(InputType& name) : infile(name)
  {}
  
  virtual bool failed_open()
  { return !infile; }
  
  virtual string get_string()
  { 
	string s;
	infile >> s;
	return s;
  }
  
  virtual int read_num()
  {
	// This function should just be "infile >> i;", however that is parsed differently in windows and linux
	// So we'll have to do it manually.
	
	/* int i;
	infile >> i;
	if(infile.fail())
    throw new parse_exception("Problem parsing number");
	return i;
	*/
	string s;
	char next_char = infile.get();
	while(isspace(next_char))
	  next_char = infile.get();
	
	if(next_char == '-')
	{
	  s+= next_char;
	  next_char = infile.get();
	}
	
	while( (next_char >= '0' && next_char <= '9'))
	{
	  s += next_char;
	  next_char = infile.get();
	}
	
	infile.putback(next_char);
	if(infile.fail() || s == "" || s == "-")
	  throw new parse_exception("Problem parsing number. Read '"+s+"'.");
	return atoi(s.c_str());
  }
  
  virtual char peek_char()
  {
	while(infile.peek() == ' ' || infile.peek() == '\n' || 
		  infile.peek() == '\r')
	  infile.get();
	
	return infile.peek();
  }
  
  /// Check if the next character from @infile is @sym.
  virtual void check_sym(char sym)
  {
	char idChar;
	infile >> idChar ;
	if(idChar != sym)
	{
	  throw new parse_exception(string("Expected '") + sym + "'. Recieved '" + idChar + "'.");
	}
  }
  
  virtual string getline()
  {
	string s;
	char* buf = new char[10000];
	infile.getline(buf,10000);
	s = buf;
	delete[] buf;
	return s;
  }
  
  /// Cleans rubbish off start of string.
  virtual void clean_string(string& s)
  {
	while(!s.empty() && isspace(s[0]))
	  s.erase(s.begin());
  }
  
  virtual string getline(char deliminator)
  {
	string s;
	char* buf = new char[10000];
	infile.getline(buf,10000, deliminator);
	s = buf;
	delete[] buf;
	clean_string(s);
	return s;
  }
  
  virtual char get_char()
  { 
	char k;
	infile >> k;
	return k;
  }
  
  
  virtual bool eof()
  { return infile.eof(); }
  
  virtual void putback(char c)
  { infile.putback(c); }
  
};

class MinionInputReader {
  void parser_info(string);
  vector< vector<Var> > Vectors ;
  vector< vector<vector<Var> > > Matrices ;
  vector< vector<vector<vector<Var> > > > Tensors ;
  vector<Var> flatten(char type, int index) ;
  vector<Var> getColOfMatrix(vector<vector<Var> >& m, int c) ;
  vector<Var> getRowThroughTensor(vector<vector<vector <Var> > >& t,int r,int c) ;
  bool readConstraint(InputFileReader* infile, bool reified) ;
  void readConstraintElement(InputFileReader* infile, const ConstraintDef&) ;
  void readConstraintTable(InputFileReader* infile, const ConstraintDef&) ;
  Var readIdentifier(InputFileReader* infile) ;
  vector< vector<Var> > readLiteralMatrix(InputFileReader* infile) ;
  vector<Var> readLiteralVector(InputFileReader* infile) ;
  void readObjective(InputFileReader* infile);
  void readTuples(InputFileReader* infile); 
  void readMatrices(InputFileReader* infile) ;
  void readValOrder(InputFileReader* infile) ;
  void readVarOrder(InputFileReader* infile) ;
  void readPrint(InputFileReader* infile) ;
  void readVars(InputFileReader* infile) ;
  vector<Var> readVectorExpression(InputFileReader* infile) ;
  
  void readGeneralConstraint(InputFileReader*, const ConstraintDef&);
 public:
  void read(char* fn) ;
  ProbSpec::CSPInstance instance;
  bool parser_verbose;
  
  MinionInputReader() : parser_verbose(false)
  {}
};
