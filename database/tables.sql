
-- Open our database
\c ascend_db;

--Enums
CREATE TYPE status AS ENUM ('available','in_use','repair','retired');

--Status
CREATE TABLE public."Status"
(
  id SERIAL PRIMARY KEY,
  status status, -- status of drone
  "long" numeric, -- longitude
  lat numeric, -- latitude
  alt numeric -- altitude
);
ALTER TABLE public."Status"
  OWNER TO ascend;
COMMENT ON TABLE public."Status"
  IS 'status of drone';
COMMENT ON COLUMN public."Status".status IS 'status of drone';
COMMENT ON COLUMN public."Status"."long" IS 'longitude';
COMMENT ON COLUMN public."Status".lat IS 'latitude';
COMMENT ON COLUMN public."Status".alt IS 'altitude';

--Battery
CREATE TABLE public."Battery"
(
  id SERIAL PRIMARY KEY, -- pid
  model text, -- model number
  volts integer, -- number of volts it outputs
  "ampHours" integer, -- number of amp hours
  charge smallint -- 1-100 percentage range charge
);
ALTER TABLE public."Battery"
  OWNER TO ascend;
COMMENT ON COLUMN public."Battery".id IS 'pid';
COMMENT ON COLUMN public."Battery".model IS 'model number';
COMMENT ON COLUMN public."Battery".volts IS 'number of volts it outputs';
COMMENT ON COLUMN public."Battery"."ampHours" IS 'number of amp hours';
COMMENT ON COLUMN public."Battery".charge IS '1-100 percentage range charge';

--Drone
CREATE TABLE public."Drone"
(
  id SERIAL PRIMARY KEY, -- drone id
  model text, -- model number for the drone
  "neededVolts" integer, -- amount of volts that is needed to power this drone
  status integer REFERENCES public."Status"(id),
  battery integer REFERENCES public."Battery"(id)

);
ALTER TABLE public."Drone"
  OWNER TO ascend;
COMMENT ON COLUMN public."Drone".id IS 'drone id';
COMMENT ON COLUMN public."Drone".model IS 'model number for the drone';
COMMENT ON COLUMN public."Drone"."neededVolts" IS 'amount of volts that is needed to power this drone';

--Jobs
CREATE TABLE public."Jobs"
(
  id SERIAL PRIMARY KEY,
  "flightFolder" text, -- file folder path
  pkg_weight smallint, -- weight of package (kg)
  pkg_width integer, -- package width in cm
  pkg_depth integer, -- depth of package (cm)
  pkg_height integer, -- height of package (cm)
  drone integer REFERENCES public."Drone"(id)
);
ALTER TABLE public."Jobs"
  OWNER TO ascend;
COMMENT ON COLUMN public."Jobs"."flightFolder" IS 'file folder path';
COMMENT ON COLUMN public."Jobs".pkg_weight IS 'weight of package (kg)';
COMMENT ON COLUMN public."Jobs".pkg_width IS 'package width in cm';
COMMENT ON COLUMN public."Jobs".pkg_depth IS 'depth of package (cm)';
COMMENT ON COLUMN public."Jobs".pkg_height IS 'height of package (cm)';




