# SQL Databases quick and dirty functions written by PaNiC
# ChangeLog:
# ver 0.0.1 - quick small functions
# ver 0.0.2 - db_insert_fields () and db_insert_update_fields()
# ver 0.0.3 - db_select_fields ()
# ver 0.0.4 - code cleanup
# ver 0.0.5 - wasted 3 hours to find the bug in db_select_fields() just a simple undef @LoH
# ver 0.1.0 - removed debug code parts


sub db_connect () {

	$dbh = Mysql->connect($db_host, $db_name);
	if (! $dbh) { return (0);}
	else { return (1); }
	
}


sub db_tables (){
	
	@tables=$dbh->listtables;
	return (@tables);
}


sub db_table_fields () {
        local ($db_table) = shift;
        $fields=$dbh->listfields($db_table);
	@names=$fields->name;
	return (@names);
}

sub query(){
	
	my ($sql)=shift;
        local ($query) = $dbh->query($sql);
        return ($query);
}


sub db_insert_fields ()
{
	local ($table) = shift;
	local (%fields) = (@_);
	my ($sql) = "insert into $table (" . join(',',keys %fields) . ") values ('" . join('\',\'', values %fields) . "')";
	$insert = &query($sql);
	return $insert->insert_id;

}

sub db_update_fields ()
{
	local ($table) = shift;
	local ($where) = shift;
	local (%fields) = (@_);
	
	@sqls = ();
	foreach $name(keys %fields) {
		if ($name ne 'id') { @sqls = ( @sqls," $name = \'$fields{$name}\' ");}
		}
			
	my ($sql) = "update $table set " . join(',',@sqls). "where $where";
	
	$update = &query($sql);
 }
	
sub db_delete_row ()
{
	
	local ($table, $id) = (@_);

	if (!$id) {
		return;
	}
	my ($sql) = "delete from $table where id=" . $id;
	$delete = &query($sql);

}

sub db_select_fields ()
{
	local ($table) = shift;
	
	
	if (@_) { $sql = shift; }
	else    { $sql = "SELECT * from $table"; }
	$select = &query($sql);
	$fields=$dbh->listfields($table);
	$results=$select->numrows;
	@names=$fields->name;
        undef @LoH;

        while (%buffer= $select->fetchhash)
        {
        	push @LoH,{ %buffer };
	 }
	
	return (@LoH);
	
}


1;