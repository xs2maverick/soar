/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package april.lcmtypes;
 
import java.io.*;
import java.nio.*;
import java.util.*;
import lcm.lcm.*;
 
public final class velodyne_t implements lcm.lcm.LCMEncodable
{
    public long utime;
    public int datalen;
    public byte data[];
 
    public velodyne_t()
    {
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0xb6befe08f6d416d3L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class>());
    }
 
    public static long _hashRecursive(ArrayList<Class> classes)
    {
        if (classes.contains(april.lcmtypes.velodyne_t.class))
            return 0L;
 
        classes.add(april.lcmtypes.velodyne_t.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        outs.writeLong(this.utime); 
 
        outs.writeInt(this.datalen); 
 
        if (this.datalen > 0)
            outs.write(this.data, 0, datalen);
 
    }
 
    public velodyne_t(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public velodyne_t(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static april.lcmtypes.velodyne_t _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        april.lcmtypes.velodyne_t o = new april.lcmtypes.velodyne_t();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        this.utime = ins.readLong();
 
        this.datalen = ins.readInt();
 
        this.data = new byte[(int) datalen];
        ins.readFully(this.data, 0, datalen); 
    }
 
    public april.lcmtypes.velodyne_t copy()
    {
        april.lcmtypes.velodyne_t outobj = new april.lcmtypes.velodyne_t();
        outobj.utime = this.utime;
 
        outobj.datalen = this.datalen;
 
        outobj.data = new byte[(int) datalen];
        if (this.datalen > 0)
            System.arraycopy(this.data, 0, outobj.data, 0, this.datalen); 
        return outobj;
    }
 
}
