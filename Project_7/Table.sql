--create table flight_tasks (
--	id serial not null primary key,
--	launchX double precision,
--	launchY double precision,
--	boomX double precision,
--	boomY double precision,
--	t0 double precision,
--	angle double precision,
--	type integer not null default 1
--);

--create table elements (
--	id serial not null primary key,
--	fid integer,
--	constraint flight_tasks_fk foreign key (fid) references flight_tasks (id),
--	launchX double precision,
--	launchY double precision,
--	boomX double precision,
--	boomY double precision,
--	tS double precision,
--	tE double precision
--);

--create table positions (
--	eid integer,
--	constraint elements_fk foreign key (eid) references elements (id),
--	time double precision,
--	x double precision,
--	y double precision
--);

select * from flight_tasks;
select * from elements;
select * from positions;